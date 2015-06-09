#ifndef _ABSTRACTPOLAR_HPP_
#define _ABSTRACTPOLAR_HPP_

#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>

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
    int heartRate = 0;

    /**
     * Update the statistics about the heart rates.
     */
    void update_statistics(const long long int& heartRate) {
        if(!heartRate) {
            std::cout << "AbstractPolar: WARNING: heartRate is ZERO!!" << std::endl;
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
     * Get a single value to represent the heart rate.
     * Also, update statistics.
     */
    virtual void updateHeartRate() = 0;

    /**
     * Print the statistics about this session.
     */
    void print_statistics(std::ostream& os = std::cout) const {
        os << "==========================" << std::endl;
        os << "|  Hearbeats Statistics  |" << std::endl;
        os << "==========================" << std::endl;
        os << "- # of records acquired: "<< stats.count << std::endl;
        os << "- Lowest Heartbeat: " << stats.lowest << std::endl;
        os << "- Greatest Heartbeat: " << stats.greatest << std::endl;
        os << "- Mean: " << static_cast<double>(stats.sum) / stats.count << std::endl;
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
