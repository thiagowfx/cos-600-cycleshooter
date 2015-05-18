#ifndef _Polar_HPP_
#define _Polar_HPP_

#include "AbstractPolar.hpp"

// C++ headers
#include <iostream>
#include <cstring>

// C headers to handle with system and serial Ports
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <paths.h>
#include <termios.h>
#include <sysexits.h>
#include <sys/param.h>
#include <sys/select.h>

#define MAX_STRING_RESPONSE 140

namespace Cycleshooter {
class Polar : public AbstractPolar {

private:
    const char *serialPort;
    int serialDescriptor;
    struct termios gOriginalTTYAttrs;

public:
    Polar();
    virtual ~Polar();
    unsigned short readInstantaneousHeartRate();
    unsigned short readMeanHeartRate();

    void setupSerialPort(const char *deviceFilePath);

private:
    // Functions to deal with the serial Port in an Unix env.
    int openSerialPort(const char *deviceFilePath);
    void closeSerialPort();

    // Function to send a command to get the number of heart rate
    // values specified in NumEntries
    bool SendGetHeartRate(int fd, int NumEntries);

    // Function to read a response string back from the HRMI
    int GetResponseString(int fd, char* ResponseString);
};

}

#endif
