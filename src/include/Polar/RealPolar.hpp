#ifndef _REALPOLAR_HPP_
#define _REALPOLAR_HPP_

#include "AbstractPolar.hpp"

// C headers to handle the system and its serial ports
#include <cerrno>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

// C++ headers
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace Cycleshooter {
/**
 * @brief The RealPolar class reads a heart rate value from a device connected to the specified serial port.
 */
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
    void sendGetHeartRate(int numEntries = 1);

    /**
     * Read a response string back from the HRMI.
     */
    void getResponseString(char* responseString);

    /**
     * Maximum number of characters that will be read until a '\r' is found.
     */
    static const int MAX_STRING_RESPONSE = 140;

    /**
     * Time to wait before trying to read from the serial port again if no chars available.
     * Measured in ms.
     */
    static const int READING_RETRY_TIME = 10;

public:
    /**
     * @brief RealPolar Construct a new RealPolar object.
     * @param deviceFilePath Path to the serial port (e.g. "/dev/ttyUSB0")
     */
    RealPolar(const char* deviceFilePath);

    /**
     * Cleanly close the serial port.
     */
    virtual ~RealPolar();

    /**
     * Get the instantaneous heart rate value from the HRMI.
     */
    virtual int getInstantaneousHeartRate();
};

}

#endif
