#ifndef _RANDOMPOLAR_HPP_
#define _RANDOMPOLAR_HPP_

#include "AbstractPolar.hpp"

namespace Cycleshooter {
/**
 * @brief The RandomPolar class generates a random heart rate value in [minPeak, maxPeak] range.
 */
class RandomPolar : public AbstractPolar {
    int minPeak, maxPeak;

public:
    RandomPolar(int minPeak = 90, int maxPeak = 100);

    /**
     * Get a random heart rate in [minPeak, maxPeak] range.
     */
    virtual int getInstantaneousHeartRate();

    void setMinPeak(const int &value);
    void setMaxPeak(const int &value);
};

}

#endif
