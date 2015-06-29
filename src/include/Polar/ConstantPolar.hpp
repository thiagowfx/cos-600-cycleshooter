#ifndef _CONSTANTPOLAR_HPP_
#define _CONSTANTPOLAR_HPP_

#include "AbstractPolar.hpp"

namespace Cycleshooter {
/**
 * @brief The ConstantPolar class generates a constant heart rate value,
 * which might be increased or decreased by a certain amount later on.
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
    virtual int getInstantaneousHeartRate() {
        update_statistics(peak);
        return peak;
    }

    /**
     * Change (increment or decrement) the peak by the specified value.
     */
    virtual void changePeaks(const int& amount) {
        peak += amount;
    }

};

}

#endif
