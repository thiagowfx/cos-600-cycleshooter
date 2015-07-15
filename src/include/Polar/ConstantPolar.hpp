#ifndef _CONSTANTPOLAR_HPP_
#define _CONSTANTPOLAR_HPP_

#include "AbstractPolar.hpp"

namespace Cycleshooter {
/**
 * @brief Generates a constant heart rate value which might be increased or decreased later on.
 */
class ConstantPolar : public AbstractPolar {
    int peak;

public:
    ConstantPolar(int peak) :
        AbstractPolar(),
        peak(peak)
    {}

    /**
     * Get the heart rate.
     */
    virtual void updateHeartRate() {
        updateStatistics(peak);
    }

    /**
     * Change (increment or decrement) the peak by the specified value.
     */
    virtual void changePeaks(const int& amount) {
        peak = std::max(0, peak + amount);
    }

};

}

#endif
