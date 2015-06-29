#include "RealPolar.hpp"

namespace Cycleshooter {

RealPolar::RealPolar(const char* deviceFilePath) :
    AbstractPolar()
{
    openSerialPort(deviceFilePath);
}

RealPolar::~RealPolar() {
    closeSerialPort();
}

void RealPolar::openSerialPort(const char* deviceFilePath) {
    std::cout << "--> RealPolar: opening serial port <--" << std::endl;

    // file descriptor for serial port
    serialDescriptor = -1;
    
    // open the serial port
    if ((serialDescriptor = open(deviceFilePath, O_RDWR | O_NOCTTY )) == -1) {
        throw std::runtime_error("Error opening serial port " + std::string(deviceFilePath) + " -- " + std::string(strerror(errno)) + "(" + std::to_string(errno) + ")");
    }
    
    // prevent other processes from opening the serial port
    if (ioctl(serialDescriptor, TIOCEXCL) == -1) {
        throw std::runtime_error("Error setting TIOCEXCL on " + std::string(deviceFilePath) + " -- " + std::string(strerror(errno)) + "(" + std::to_string(errno) + ")");
    }
    
    // get the current serial port options and save them to restore on exit
    if (tcgetattr(serialDescriptor, &originalTTYAttributes) == -1) {
        throw std::runtime_error("Error getting tty attributes on " + std::string(deviceFilePath) + " -- " + std::string(strerror(errno)) + "(" + std::to_string(errno) + ")");
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
        throw std::runtime_error("Error setting tty attributes on " + std::string(deviceFilePath) + " -- " + std::string(strerror(errno)) + "(" + std::to_string(errno) + ")");
    }
}

void RealPolar::closeSerialPort() {
    std::cout << "--> RealPolar: closing serial port <--" << std::endl;

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

void RealPolar::sendGetHeartRate(int numEntries) {
    // validate numEntries
    if (numEntries < 0)
        numEntries = 0;
    else if (numEntries > 32)
        numEntries = 32;

    // array sized to hold the largest command string
    char sendCommand[8];

    // number of characters in the command string
    int cmdLength;
    
    // build the command string
    // note: "\015" is the carriage return character ('\r')
    cmdLength = sprintf(sendCommand, "G%0d\015", numEntries);
    
    // send the command string
    if (write(serialDescriptor, sendCommand, cmdLength) != cmdLength) {
        throw std::runtime_error("Error sending the command string -- " + std::string(strerror(errno)) + "(" + std::to_string(errno) + ")");
    }
}

void RealPolar::getResponseString(char* responseString){
    char b[2];
    int i = 0;
    
    do {
        // read one char at a time
        int n = read(serialDescriptor, b, 1);

        if (n == -1) {
            throw std::runtime_error("Error getting the response string  -- " + std::string(strerror(errno)) + "(" + std::to_string(errno) + ")");
        }

        // no chars available for reading right now
        if (n == 0) {
            // wait a little bit before trying again
            usleep(READING_RETRY_TIME * 1e3);
            continue;
        }
        
        // store the character
        responseString[i++] = b[0];
        
        // repeat until we see the <CR> ('\r') character or exceed the buffer
    } while ((b[0] != 0x0D) && (i < MAX_STRING_RESPONSE));
    
    // null terminate the string (replace the <CR>)
    responseString[i-1] = 0;
}

int RealPolar::getInstantaneousHeartRate() {
    // send a get heart Rate command requesting history buffer entries
    sendGetHeartRate();

    // response string
    char *rspBytes = new char[MAX_STRING_RESPONSE];
    getResponseString(rspBytes);

    int heartRate;

    // discard the two first values (the heart rate is the third one)
    sscanf(rspBytes, "%*u %*u %u", &heartRate);

    delete rspBytes;

    updateStatistics(heartRate);

    return heartRate;
}

}
