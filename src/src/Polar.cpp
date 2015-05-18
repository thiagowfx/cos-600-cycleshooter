#include "Polar.hpp"

namespace Cycleshooter {

Polar::Polar() {

}

Polar::~Polar() {

}

unsigned short Polar::readInstantaneousHeartRate(){

}

unsigned short Polar::readMeanHeartRate(){

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

void Polar::closeSerialPort(){
    
    int fd = serialDescriptor;
    
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

bool Polar::SendGetHeartRate(int fd, int NumEntries){

}

int Polar::GetResponseString(int fd, char* ResponseString){

}

void Polar::setupSerialPort(const char *deviceFilePath){
    serialPort = deviceFilePath;
}

}



