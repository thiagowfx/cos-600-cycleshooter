#ifndef _ABSTRACTPOLAR_HPP_
#define _ABSTRACTPOLAR_HPP_

#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>

#include "Logging.hpp"

namespace Cycleshooter {

class AbstractPolar {

private:
    struct {
        /**
        * Sum of the values acquired in this session.
        */
        long long int sum = 0;

        /**
        * Number of the values acquired in this session.
        */
        long long int count = 0;

        /**
        * Maximum value of a heart beat in this session.
        */
        long long int greatest = std::numeric_limits<long long int>::min();

        /**
        * Minimum value of a heart beat in this session.
        */
        long long int lowest = std::numeric_limits<long long int>::max();
    } stats;

protected:

    /**
     * The last obtained heartRate.
     */
    int heartRate;

    /**
     * Update the statistics about the heart rates.
     */
    void updateStatistics(const long long int& heartRate) {
        if(!heartRate) {
            LOG_FATAL("Heartrate is ZERO");
            return;
        }

        this->heartRate = heartRate;
        stats.lowest = std::min(stats.lowest, heartRate);
        stats.greatest = std::max(stats.greatest, heartRate);
        stats.sum += heartRate;
        ++stats.count;
    }

public:
    AbstractPolar(){}

    /**
     * Acquire the heart rate and update statistics.
     */
    virtual void updateHeartRate() = 0;

    /**
     * Print statistics of this session.
     */
    void printStatistics(std::ostream& os = std::cout) const {
        os << "* Hearbeat Statistics\n"
              "- # of records acquired: " << stats.count << std::endl <<
              "- Lowest Heartbeat: " << stats.lowest << std::endl <<
              "- Greatest Heartbeat: " << stats.greatest << std::endl <<
              "- Mean: " << static_cast<double>(stats.sum) / stats.count << std::endl;
    }

    /**
     * Change (increment or decrement) the peaks by the specified value.
     */
    virtual void changePeaks(const int& amount) {}

    /**
     * Return the last obtained heart rate.
     */
    int getHeartRate() const {
        return heartRate;
    }
};

}

#endif
