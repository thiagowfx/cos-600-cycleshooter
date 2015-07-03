#ifndef _REALBICYCLE_HPP_
#define _REALBICYCLE_HPP_

#include "AbstractBicycle.hpp"

// C headers to handle the system and its serial ports
#include <cerrno>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

//C++ headers
#include <cstring>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <sstream>

namespace Cycleshooter {
/**
 * @brief The RealBicycle class acts as a bridge between the
 * controller circuit of the bicycle and our game. It reads and writes
 * from it by receiving and sending appropriate strings.
 */
class RealBicycle : public AbstractBicycle {

    /**
     * Serial port file descriptor.
     */
    int serialDescriptor = -1;

    /**
     * Original attributes of the serial port.
     * Saved temporarily to be restored in the end.
     */
    struct termios originalTTYAttributes;

    /**
     * Time between consecutive reads.
     */
    static const unsigned READING_RETRY_TIME_MS = 10;

    /**
     * Maximun number of characters that might be read at a time.
     */
    static const int MAX_STRING_RESPONSE = 16;

    /**
     * Open the serial port in the specified path.
     */
    void openSerialPort(const char* deviceFilePath) {
        std::cout << "--> RealBicycle: opening serial port <--" << std::endl;

        // open the serial port
        if ((serialDescriptor = open(deviceFilePath, O_RDWR | O_NOCTTY )) == -1) {
            throw std::runtime_error("----> RealBicycle: Error opening serial port " + std::string(deviceFilePath) + " -- " + std::string(strerror(errno)) + "(" + std::to_string(errno) + ")");
        }

        // prevent other processes from opening the serial port
        if (ioctl(serialDescriptor, TIOCEXCL) == -1) {
            throw std::runtime_error("----> RealBicycle: Error setting TIOCEXCL on " + std::string(deviceFilePath) + " -- " + std::string(strerror(errno)) + "(" + std::to_string(errno) + ")");
        }

        // get the current serial port options and save them to restore on exit
        if (tcgetattr(serialDescriptor, &originalTTYAttributes) == -1) {
            throw std::runtime_error("----> RealBicycle: Error getting tty attributes on " + std::string(deviceFilePath) + " -- " + std::string(strerror(errno)) + "(" + std::to_string(errno) + ")");
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
            throw std::runtime_error("----> RealBicycle: Error setting tty attributes on " + std::string(deviceFilePath) + " -- " + std::string(strerror(errno)) + "(" + std::to_string(errno) + ")");
        }
    }

    /**
     * Close the serial port.
     */
    void closeSerialPort() {
        std::cout << "--> RealBicycle: closing serial port <--" << std::endl;

        // block until all written output has been sent from the device
        if (tcdrain(serialDescriptor) == -1) {
            std::cout << "----> RealBicycle: Error waiting for drain - "<< strerror(errno) << "(" << errno <<")" << std::endl;
        }

        // reset the serial port back to the state in which we found it
        if (tcsetattr(serialDescriptor, TCSANOW, &originalTTYAttributes) == -1) {
            std::cout << "----> RealBicycle: Error restoring tty attributes - "<< strerror(errno) << "(" << errno <<")" << std::endl;
        }

        // close the port
        close(serialDescriptor);
    }

    /**
     * @brief readSerialPort
     */
    std::string readFromSerialPort() {
        // TODO: review this whole function

        /*
         * String format: B000000
         */

        char detectB[2];
        int i = 0;

        do {
            int n = read(serialDescriptor, detectB, 1);

            // if serial is not available
            if (n == -1) {
                throw std::runtime_error("----> RealBicycle: Error getting the response string from the bycicle -- " + std::string(strerror(errno)) + "(" + std::to_string(errno) + ")");
            }

            if (n == 0) {
                // wait a little bit before trying again
                usleep(READING_RETRY_TIME_MS * 1e3);
                continue;
            }

            ++i;
            if(i > MAX_STRING_RESPONSE) {
                throw std::runtime_error("----> RealBicycle: Error: read more than MAX_STRING_RESPONSE characters -- " + std::string(strerror(errno)) + "(" + std::to_string(errno) + ")");
            }

        } while(detectB[0] != 'B');

        // from here we wave to
        // (1) read 6 digits
        // and (2) return them concatenated as a std::string, plus add a "B" before them

        char digit[6 + 1];

        do {
            int n = read(serialDescriptor, digit, 6);

            // if serial is not available
            if (n == -1) {
                throw std::runtime_error("----> RealBicycle: Error getting the response string from the bycicle (2) -- " + std::string(strerror(errno)) + "(" + std::to_string(errno) + ")");
            }

            // TODO: maybe remove this if
            if (n == 0) {
                // wait a little bit before trying again
                usleep(READING_RETRY_TIME_MS * 1e3);
                continue;
            }

            // TODO: maybe compare n with 6 for further validation (see read(2))
        } while(false);

        return "B" + std::string(digit, digit + 6);
        // TODO: for further testing, if necessary: assert(std::string("B" + std::string(digit, digit + 6)).size() == 7);
    }


//        char* responseString;
//        char b[2];
//        int i = 0;

//        do {
//            if(i == 0 && b[0] != 'B'){
//                responseString[i++] = b[0];
//            }

//            // store the character
//            responseString[i++] = b[0];


//        } while((b[0] != 0x0D) && (i < MAX_STRING_RESPONSE));

//        // null terminate the string (replace the <CR>)
//        responseString[i-1] = 0;

//        return responseString;
//    }

    /**
     * @brief writeSerialPort
     */
    void writeToSerialPort(const std::string& command) {
        std::cout << "--> RealBicycle: Writing to Serial Port: " << command << " <-- " << std::endl;

        if (write(serialDescriptor, command.c_str(), command.size()) != command.size()) {
            throw std::runtime_error("----> RealBicycle: Error sending the command string -- " + std::string(strerror(errno)) + "(" + std::to_string(errno) + ")");
        }
    }

public:
    RealBicycle(const char* deviceFilePath) :
        AbstractBicycle()
    {
        openSerialPort(deviceFilePath);
        setFriction(0);
    }

    virtual ~RealBicycle() {
        //setFriction(0);
        closeSerialPort();
    }

    /**
     * Update the speed value of the bicycle, by reading it from the controller circuit.
     */
    virtual void updateSpeed() {
        std::cout << "--> RealBicycle: Updating Speed" << std::endl;

        std::string response = readFromSerialPort();
        std::cout << "----> RealBicycle: Read response: " << response << std::endl;

        speed = atoi(response.substr(1,3).c_str());
        std::cout << "----> RealBicycle: Read speed: " << speed << std::endl;

        update_statistics(speed);
    }

    /**
     * Update the friction of the bicycle by sending a command to it.
     */
    virtual void setFriction(const int& value) {
        std::cout << "--> RealBicycle: Setting Friction: " << value << " <--" << std::endl;

        /*
         * String command to change the load of the bicycle:
         * '\xA0' '0' '0' '0' '0' ==> "A0000" (it is not an 'A', use the hex code)
         * For example: 'A0125'
         */
        std::stringstream command;
        command << "\xA0" << std::setfill('0') << std::setw(4) << value;

        friction = value;
        writeToSerialPort(command.str());
    }

    virtual void changeFriction(const int& value) {
        friction = std::max(0, friction + value);
        friction = std::min(400, friction);
        setFriction(friction);
    }

};
}

#endif
