#ifndef _ABSTRACTPOLAR_HPP_
#define _ABSTRACTPOLAR_HPP_

#include <algorithm>
#include <iostream>
#include <cstring>

namespace Cycleshooter {
class AbstractPolar {
protected:
    /**
     * Sum of the values acquired in this section.
     */
    long long int sum = 0;

    /**
     * Number of the values of acquired in this section.
     */
    long long int count = 0;
    /**
     * Max value of a heart beat.
     */
    long long int max;
    /**
     * Min value of a heart beat.
     */
    long long int min;

    void updateStatistics(int heartRate);

public:
    AbstractPolar();

    /**
     * Get a single value to represent the heart rate and record it.
     */
    virtual int getInstantaneousHeartRate() = 0;

    /**
     * Get the mean of the recorded heart rates.
     * Useful to get the base heart rate in the beginning of the game.
     */
    std::string printStatistics();
};

}

#endif
