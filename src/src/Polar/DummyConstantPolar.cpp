#include "DummyConstantPolar.hpp"

namespace Cycleshooter {

DummyConstantPolar::DummyConstantPolar(unsigned constantHeartRate) :
    constantHeartRate(constantHeartRate)
{
}

int DummyConstantPolar::getInstantaneousHeartRate() {
    return constantHeartRate;
}

int DummyConstantPolar::getMeanHeartRate() {
    return constantHeartRate;
}

void DummyConstantPolar::setConstantHeartRate(const unsigned &value) {
    constantHeartRate = value;
}

}
