#ifndef _CONSTANTPOLAR_HPP_
#define _CONSTANTPOLAR_HPP_

#include "RandomPolar.hpp"

namespace Cycleshooter {
class ConstantPolar : public RandomPolar {
public:
    ConstantPolar(unsigned peak = 90);
};
}

#endif
