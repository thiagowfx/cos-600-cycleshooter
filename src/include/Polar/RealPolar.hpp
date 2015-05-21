#ifndef _REALPOLAR_HPP_
#define _REALPOLAR_HPP_

#include "AbstractPolar.hpp"

// C++ headers
#include <iostream>
#include <cstring>
#include <cstdio>
#include <stdexcept>
#include <vector>

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
class RealPolar : public AbstractPolar {
    /**
     * Serial port file descriptor.
     */
    int serialDescriptor;

    /**
     * Original attributes of the serial port.
     * Saved temporarily to be restored in the end.
     */
    struct termios originalTTYAttributes;

    /**
     * Open the serial port in the specified path.
     */
    void openSerialPort(const char* deviceFilePath);

    /**
     * Close the serial port.
     */
    void closeSerialPort();

    /**
     * Send a command to get the specified number of heart rate values.
     */
    int sendGetHeartRate(int NumEntries);

    /**
     * Read a response string back from the HRMI.
     */
    int getResponseString(char* ResponseString);

public:
    RealPolar(const std::string& deviceFilePath, unsigned HRHistoryLimit = 120);
    virtual ~RealPolar();

    /**
     * Get the instantaneous value from the HRMI.
     */
    virtual unsigned getInstantaneousHeartRate();
};

}

#endif
