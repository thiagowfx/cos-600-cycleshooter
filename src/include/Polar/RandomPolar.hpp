#ifndef _RANDOMPOLAR_HPP_
#define _RANDOMPOLAR_HPP_

#include "AbstractPolar.hpp"

namespace Cycleshooter {
/**
 * @brief The RandomPolar class generates a random heart rate value in [minPeak, maxPeak] range.
 */
class RandomPolar : public AbstractPolar {
    unsigned minPeak, maxPeak;

public:
    RandomPolar(unsigned minPeak = 90, unsigned maxPeak = 100, unsigned HRHistoryLimit = 120);

    /**
     * Get a random heart rate in [minPeak, maxPeak] range.
     */
    virtual unsigned getInstantaneousHeartRate();

    void setMinPeak(const unsigned &value);
    void setMaxPeak(const unsigned &value);
};

}

#endif
