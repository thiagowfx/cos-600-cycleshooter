#include "RealPolar.hpp"

namespace Cycleshooter {

RealPolar::RealPolar(const std::string &deviceFilePath, unsigned HRHistoryLimit) :
    AbstractPolar(HRHistoryLimit)
{
    openSerialPort(deviceFilePath.c_str());
}

RealPolar::~RealPolar() {
    closeSerialPort();
}

void RealPolar::openSerialPort(const char* deviceFilePath) {
    // file descriptor for serial port
    serialDescriptor = -1;
    
    // open the serial port
    if ((serialDescriptor = open(deviceFilePath, O_RDWR | O_NOCTTY )) == -1) {
        std::cout << "Error opening serial port " << deviceFilePath << " - " << strerror(errno) << "(" << errno << ")" << std::endl;
        throw std::runtime_error("Error opening serial port");
    }
    
    // prevent other processes from opening the serial port
    if (ioctl(serialDescriptor, TIOCEXCL) == -1) {
        std::cout << "Error setting TIOCEXCL on " << deviceFilePath << " - " << strerror(errno) << "(" << errno << ")" << std::endl;
        throw std::runtime_error("Error opening serial port");
    }
    
    // get the current serial port options and save them to restore on exit
    if (tcgetattr(serialDescriptor, &originalTTYAttributes) == -1) {
        std::cout << "Error getting tty attributes " << deviceFilePath << " - " << strerror(errno) << "(" << errno << ")" << std::endl;
        throw std::runtime_error("Error opening serial port");
    }

    // serial port configuration options
    struct termios options = originalTTYAttributes;

    // set raw input (non-canonical) mode, with reads blocking until either
    // a single character has been received, or a one-second timeout expires
    cfmakeraw(&options);
    options.c_cc[VMIN] = 1;
    options.c_cc[VTIME] = 10;

    // set the baud rate and word length
    cfsetspeed(&options, B9600);
    options.c_cflag |= (CS8);
    
    // cause new options to take effect immediately
    if (tcsetattr(serialDescriptor, TCSANOW, &options) == -1) {
        std::cout << "Error setting tty attributes!" << std::endl;
        throw std::runtime_error("Error opening serial port");
    }
}

void RealPolar::closeSerialPort() {
    // block until all written output has been sent from the device
    if (tcdrain(serialDescriptor) == -1) {
        std::cout << "Error waiting for drain - "<< strerror(errno) << "(" << errno <<")" << std::endl;
    }
    
    // reset the serial port back to the state in which we found it
    if (tcsetattr(serialDescriptor, TCSANOW, &originalTTYAttributes) == -1) {
        std::cout << "Error restoring tty attributes - "<< strerror(errno) << "(" << errno <<")" << std::endl;
    }
    
    // close the port
    close(serialDescriptor);
}

int RealPolar::sendGetHeartRate(int numEntries) {
    // array sized to hold the largest command string
    char sendCommand[8];

    // number of characters in the command string
    int cmdLength;
    
    // validate numEntries
    if (numEntries < 0)
        numEntries = 0;
    else if (numEntries > 32)
        numEntries = 32;
    
    // build the command string
    // note: "\015" is the carriage return character
    cmdLength = sprintf(sendCommand, "G%0d\015", numEntries);
    
    // send the command string
    return write(serialDescriptor, sendCommand, cmdLength) == cmdLength;
}

int RealPolar::getResponseString(char* responseString){
    char b[2];
    int i = 0;
    
    do {
        // read a char at a time
        int n = read(serialDescriptor, b, 1);

        if (n == -1) {
            // read failed
            return ERROR_CODE;
        }

        if (n == 0) {
            // wait 10 msec before trying again
            usleep(10 * 1000);
            continue;
        }
        
        // store the character
        responseString[i] = b[0];
        ++i;
        
        // repeat until we see the <CR> character or exceed the buffer
    } while ((b[0] != 0x0D) && (i < MAX_STRING_RESPONSE));
    
    // null terminate the string (replace the <CR>)
    responseString[i-1] = 0;

    return 0;
}

unsigned RealPolar::getInstantaneousHeartRate() {
    // response string
    char *rspBytes = new char[MAX_STRING_RESPONSE];
    int numEntries = 1;
    
    // send a Get Heart Rate command requesting history buffer entries
    if (sendGetHeartRate(numEntries) == 0) {
        std::cout << "Error: SendGetHeartRate failed!" << std::endl;
        return ERROR_CODE;
    }
    
    if (getResponseString(rspBytes) == -1) {
        std::cout << "Error: GetResponseString failed!" << std::endl;
        return ERROR_CODE;
    }
    else {
        std::cout << "Request => " << rspBytes << std::endl;
    }

    unsigned heartRate = atoi(rspBytes);
    addRecord(heartRate);
    return heartRate;
}

}
