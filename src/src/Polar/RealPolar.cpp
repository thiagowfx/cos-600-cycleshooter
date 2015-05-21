#include "RealPolar.hpp"

namespace Cycleshooter {

RealPolar::RealPolar() {
    // Initialize the size of the vector to get the mean value
    itrMeanHeartRate = 20;
    // Clear the vector (Guarantee the vector initialize empty)
    recordHeartRate.clear();
}

RealPolar::~RealPolar() {
   // Clear the vector and de-alloc the pointer
    recordHeartRate.clear();
    delete serialPort;
}

void RealPolar::setupSerialPort(const char *deviceFilePath){
    serialPort = deviceFilePath;
}

int RealPolar::openSerialPort(const char *deviceFilePath){
    // variables
    int fd = -1;              // file descriptor for serial port
    struct termios options;   // serial port configuration options

    // Open the serial port
    if ((fd = open(deviceFilePath, O_RDWR | O_NOCTTY )) == -1) {
        std::cout << "Error opening serial port " << deviceFilePath << " - " <<
                strerror(errno) << "(" << errno << ")" << std::endl;
        return ERROR_CODE;
    }

    // Prevent other processes from opening the serial port
    if (ioctl(fd, TIOCEXCL) == -1) {
        std::cout << "Error setting TIOCEXCL on " << deviceFilePath << " - " <<
                strerror(errno) << "(" << errno << ")" << std::endl;
        return ERROR_CODE;
    }

    // Get the serial port current options and save them to restore on exit
    if (tcgetattr(fd, &gOriginalTTYAttrs) == -1) {
        std::cout << "Error getting tty attributes " << deviceFilePath << " - " <<
                strerror(errno) << "(" << errno << ")" << std::endl;
        return ERROR_CODE;
    }

    // Configure the serial port
    options = gOriginalTTYAttrs;
    // Set raw input (non-canonical) mode, with reads blocking until either a
    // single character has been received or a one second timeout expires
    cfmakeraw(&options);
    options.c_cc[VMIN] = 1;
    options.c_cc[VTIME] = 10;
    // Set the baud rate and word length
    cfsetspeed(&options, B9600);
    options.c_cflag |= (CS8);

    // Cause the new options to take effect immediately
    if (tcsetattr(fd, TCSANOW, &options) == -1) {
        std::cout << "Error setting tty attributes!" << std::endl;
        return ERROR_CODE;
    }
    // Saving the state of the file descriptor (To close the serial in the future)
    serialDescriptor = fd;
    return(fd);
}

void RealPolar::closeSerialPort(int fd){
    
    // Block until all written output has been sent from the device
    if (tcdrain(fd) == -1) {
        std::cout << "Error waiting for drain - "<< strerror(errno) << "(" <<
         errno <<")" << std::endl;
    }

    // Reset the serial port back to the state in which we found it
    if (tcsetattr(fd, TCSANOW, &gOriginalTTYAttrs) == -1) {
        std::cout << "Error restoring tty attributes - "<< strerror(errno) << "(" <<
         errno <<")" << std::endl;
    }

    // Close the port
    close(fd);
}

int RealPolar::sendGetHeartRate(int fd, int numEntries){
    char sendCommand[8];      // Array sized to hold the largest command string
    int  cmdLength;           // Number of characters in the command string

    // Validate NumEntries
    if (numEntries < 0)
        numEntries = 0;
    else if (numEntries > 32)
        numEntries = 32;

    // Build the command string
    //   Note: "\015" is the carriage return character
    cmdLength = sprintf(sendCommand, "G%0d\015", numEntries);

    // Send the command string
    return(write(fd, sendCommand, cmdLength) == cmdLength);
}

int RealPolar::getResponseString(int fd, char* responseString){
    char b[2];
    int i = 0;

    do {
        int n = read(fd, b, 1);     // read a char at a time
        if (n == -1)
            return ERROR_CODE;      // read failed
        if (n == 0) {
            usleep(10 * 1000);      // wait 10 msec before trying again
            continue;
        }

        responseString[i] = b[0];   // store the character
        i++;

    // repeat until we see the <CR> character or exceed the buffer
    } while ((b[0] != 0x0D) && (i < MAX_STRING_RESPONSE));

    // null terminate the string (replace the <CR>)
    responseString[i-1] = 0;
    return(0);
}

unsigned RealPolar::getInstantaneousHeartRate(){
    
    char *rspBytes = new char[MAX_STRING_RESPONSE]; // Response string
    short heartRate;
    int numEntries = 1;

    // Open the serial port device associated with the HRMI
    if (openSerialPort(serialPort) == -1) {
        return ERROR_CODE;
    }

    // Send a Get Heart Rate command requesting history buffer entries
    if (sendGetHeartRate(serialDescriptor, numEntries) == 0) {
        std::cout << "Error: SendGetHeartRate failed!" << std::endl;
        return ERROR_CODE;
    }

    if (getResponseString(serialDescriptor, rspBytes) == -1) {
        std::cout << "Error: GetResponseString failed!" << std::endl;
        return ERROR_CODE;
    } else {
        std::cout << "Request => " << rspBytes << std::endl;
    }

    closeSerialPort(serialDescriptor);
    heartRate = atoi(rspBytes);

    if(recordHeartRate.size() < itrMeanHeartRate){
        recordHeartRate.push_back(heartRate);
    } else {
        recordHeartRate.erase(recordHeartRate.begin() + 1);
        recordHeartRate.push_back(heartRate);
    }

    return heartRate;
}

}
