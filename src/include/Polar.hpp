#ifndef _Polar_HPP_
#define _Polar_HPP_

#include "AbstractPolar.hpp"

namespace Cycleshooter {
class Polar : public AbstractPolar {

public:
    Polar();
    virtual ~Polar();
    unsigned short readInstantaneousHeartRate();
    unsigned short readMeanHeartRate();
};

}

#endif
