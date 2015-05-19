#ifndef _DummyPolar_HPP_
#define _DummyPolar_HPP_

#include "AbstractPolar.hpp"

#include <iostream>
#include <cstdlib>
#include <vector>

namespace Cycleshooter {
class DummyPolar : public AbstractPolar {
public:
    int itrMeanHeartRate;

private:
    short minHeartPeak;
    short maxHeartPeak;
    std::vector<int> recordHeartRate;

public:
    DummyPolar();
    virtual ~DummyPolar();

    // Setup the min and max values to rand.
    // Tip : Use short intervals (e.g, 80 - 90)
    void setupHeartPeaks(short min, short max);

    // Get a single value to represent the Heart Rate and save it
    short readInstantaneousHeartRate();

    // Get the mean of "itrMeanHeartRate" values.
    //Useful to get the base Heart Rate in the beginning of the game
    short readMeanHeartRate();

};

}

#endif
