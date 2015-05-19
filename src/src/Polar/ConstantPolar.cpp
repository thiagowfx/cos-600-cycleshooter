#include "ConstantPolar.hpp"

namespace Cycleshooter {

ConstantPolar::ConstantPolar(unsigned constantHeartRate) :
    constantHeartRate(constantHeartRate)
{
}

int ConstantPolar::getInstantaneousHeartRate() {
    return constantHeartRate;
}

int ConstantPolar::getMeanHeartRate() {
    return constantHeartRate;
}

void ConstantPolar::setConstantHeartRate(const unsigned &value) {
    constantHeartRate = value;
}

}
