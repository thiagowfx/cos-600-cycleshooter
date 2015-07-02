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
    RandomPolar(int minPeak, int maxPeak) :
        AbstractPolar(),
        minPeak(minPeak),
        maxPeak(maxPeak)
    {}

    /**
     * Get a random heart rate in [minPeak, maxPeak] range.
     */
    virtual void updateHeartRate() {
        auto heartRate = minPeak + (rand() % (maxPeak - minPeak + 1));
        update_statistics(heartRate);
    }

    /**
     * Change (increment or decrement) the maximum and the minimum peak by the specified value.
     */
    virtual void changePeaks(const int& amount) {
        if(minPeak + amount < 0)
            return;

        minPeak += amount;
        maxPeak += amount;
    }

};

}

#endif
