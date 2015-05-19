#ifndef _DUMMYCONSTANTPOLAR_HPP_
#define _DUMMYCONSTANTPOLAR_HPP_

#include "AbstractPolar.hpp"

namespace Cycleshooter {
class ConstantPolar : public AbstractPolar {
    unsigned constantHeartRate;

public:
    ConstantPolar(unsigned constantHeartRate = 90);

    virtual int getInstantaneousHeartRate();
    virtual int getMeanHeartRate();
    void setConstantHeartRate(const unsigned &value);
};
}

#endif
