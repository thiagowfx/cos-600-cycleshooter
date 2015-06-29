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
     * Update the statistics about the heart rates.
     */
    void update_statistics(const long long int& heartRate);

public:
    AbstractPolar(){};

    /**
     * Get a single value to represent the heart rate.
     * Also, update statistics.
     */
    virtual int getInstantaneousHeartRate() = 0;

    /**
     * Print the statistics about this session.
     */
    void print_statistics(std::ostream& os = std::cout) const;

    /**
     * Change (increment or decrement) the peaks by the specified value.
     */
    virtual void changePeaks(const int& amount) {}
};

}

#endif
