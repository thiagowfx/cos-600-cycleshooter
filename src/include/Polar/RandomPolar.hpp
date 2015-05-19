#ifndef _DUMMYPOLAR_HPP_
#define _DUMMYPOLAR_HPP_

#include "AbstractPolar.hpp"

#include <cstdlib>
#include <deque>
#include <iostream>

namespace Cycleshooter {
class RandomPolar : public AbstractPolar {
    unsigned minPeak, maxPeak;
    const unsigned historyLimit;
    std::deque<unsigned> heartRateHistory;

    // TODO: move this to abstract class
    void addRecord(const unsigned& record);

public:
    RandomPolar(unsigned minPeak = 85, unsigned maxPeak = 95, unsigned historyLimit = 200);

    /**
      * Get a single value to represent the Heart Rate and save it
      */
    virtual int getInstantaneousHeartRate();

    /** Get the mean of "itrMeanHeartRate" values.
      * Useful to get the base Heart Rate in the beginning of the game
      */
    virtual int getMeanHeartRate();

    void setMinHeartPeak(const unsigned &value);
    void setMaxHeartPeak(const unsigned &value);
};

}

#endif
