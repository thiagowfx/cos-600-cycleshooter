#ifndef _DummyPolar_HPP_
#define _DummyPolar_HPP_

#include "AbstractPolar.hpp"

namespace Cycleshooter {
class DummyPolar : public AbstractPolar {
public:
    int itrMeanHeartRate;

private:
    short minHeartPeak;
    short maxHeartPeak;

public:
    DummyPolar();
    virtual ~DummyPolar();

    void setupHeartPeaks(short min, short max);

    short readInstantaneousHeartRate();
    short readMeanHeartRate();

private:

};

}

#endif
