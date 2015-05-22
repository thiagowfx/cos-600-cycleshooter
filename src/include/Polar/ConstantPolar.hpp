#ifndef _CONSTANTPOLAR_HPP_
#define _CONSTANTPOLAR_HPP_

#include "RandomPolar.hpp"

namespace Cycleshooter {
/**
 * @brief The ConstantPolar class generates a constant heart rate value.
 */
class ConstantPolar : public RandomPolar {
public:
    ConstantPolar(unsigned peak = 90);
};
}

#endif
