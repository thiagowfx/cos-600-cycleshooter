#ifndef _Polar_HPP_
#define _Polar_HPP_

#include "AbstractPolar.hpp"

// C++ headers
#include <iostream>
#include <cstring>
#include <cstdio>

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
#define ERROR_CODE -1

namespace Cycleshooter {
class Polar : public AbstractPolar {

public:
    int itrMean = 20;

private:
    const char *serialPort;
    int serialDescriptor;
    struct termios gOriginalTTYAttrs;

public:
    Polar();
    virtual ~Polar();
    // Get the instantaneous value from the HRMI
    short readInstantaneousHeartRate();
    // Get the mean of "itrMean" values. 
    //Useful to get the base Heart Rate in the beginning of the game
    short readMeanHeartRate();

    void setupSerialPort(const char *deviceFilePath);

private:
    // Functions to deal with the serial Port in an Unix env.
    int openSerialPort(const char *deviceFilePath);
    void closeSerialPort(int fd);

    // Function to send a command to get the number of heart rate
    // values specified in NumEntries
    int sendGetHeartRate(int fd, int NumEntries);

    // Function to read a response string back from the HRMI
    int getResponseString(int fd, char* ResponseString);
};

}

#endif
