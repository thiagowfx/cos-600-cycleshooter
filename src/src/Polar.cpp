#include "Polar.hpp"

namespace Cycleshooter {

Polar::Polar() {

}

Polar::~Polar() {

}

void Polar::setupSerialPort(const char *deviceFilePath){
    serialPort = deviceFilePath;
}

int Polar::openSerialPort(const char *deviceFilePath){
    // variables
    int fd = -1;              // file descriptor for serial port
    struct termios options;   // serial port configuration options

    // Open the serial port
    if ((fd = open(deviceFilePath, O_RDWR | O_NOCTTY )) == -1) {
        std::cout << "Error opening serial port " << deviceFilePath << " - " <<
                strerror(errno) << "(" << errno << ")" << std::endl;
        return(-1);
    }

    // Prevent other processes from opening the serial port
    if (ioctl(fd, TIOCEXCL) == -1) {
        std::cout << "Error setting TIOCEXCL on " << deviceFilePath << " - " <<
                strerror(errno) << "(" << errno << ")" << std::endl;
        return(-1);
    }

    // Get the serial port current options and save them to restore on exit
    if (tcgetattr(fd, &gOriginalTTYAttrs) == -1) {
        std::cout << "Error getting tty attributes" << deviceFilePath << " - " <<
                strerror(errno) << "(" << errno << ")" << std::endl;
        return(-1);
    }

    // Configure the serial port
    options = gOriginalTTYAttrs;
    //   Set raw input (non-canonical) mode, with reads blocking until either a
    //   single character has been received or a one second timeout expires
    cfmakeraw(&options);
    options.c_cc[VMIN] = 1;
    options.c_cc[VTIME] = 10;
    //    Set the baud rate and word length
    cfsetspeed(&options, B9600);
    options.c_cflag |= (CS8);

    //    Cause the new options to take effect immediately
    if (tcsetattr(fd, TCSANOW, &options) == -1) {
        std::cout << "Error setting tty attributes!" << std::endl;
        return(-1);
    }
    // Saving the state of the file descriptor (To close the serial in the future)
    serialDescriptor = fd;
    return(fd);
}

void Polar::closeSerialPort(int fd){
    
    // Block until all written output has been sent from the device
    if (tcdrain(fd) == -1) {
        std::cout << "Error waiting for drain - " << strerror(errno) << "(" <<
         errno <<")" << std::endl;
    }

    // Reset the serial port back to the state in which we found it
    if (tcsetattr(fd, TCSANOW, &gOriginalTTYAttrs) == -1) {
        std::cout << "Error restoring tty attributes - " << strerror(errno) << "(" <<
         errno <<")" << std::endl;
    }

    // Close the port
    close(fd);
}

int Polar::SendGetHeartRate(int fd, int NumEntries){
    char SendCommand[8];      // Array sized to hold the largest command string
    int  CmdLength;           // Number of characters in the command string

    // Validate NumEntries
    if (NumEntries < 0)
        NumEntries = 0;
    else if (NumEntries > 32)
        NumEntries = 32;

    // Build the command string
    //   Note: "\015" is the carriage return character
    CmdLength = sprintf(SendCommand, "G%0d\015", NumEntries);

    // Send the command string
    return(write(fd, SendCommand, CmdLength) == CmdLength);
}

int Polar::GetResponseString(int fd, char* ResponseString){
    char b[2];
    int i = 0;

    do {
        int n = read(fd, b, 1);     // read a char at a time
        if (n == -1)
            return(-1);             // read failed
        if (n == 0) {
            usleep(10 * 1000);      // wait 10 msec before trying again
            continue;
        }

        ResponseString[i] = b[0];   // store the character
        i++;

    // repeat until we see the <CR> character or exceed the buffer
    } while ((b[0] != 0x0D) && (i < MAX_STRING_RESPONSE));

    // null terminate the string (replace the <CR>)
    ResponseString[i-1] = 0;
    return(0);
}

unsigned short Polar::readInstantaneousHeartRate(){

}

unsigned short Polar::readMeanHeartRate(){

}

}



