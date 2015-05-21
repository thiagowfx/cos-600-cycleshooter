#include "RandomPolar.hpp"

namespace Cycleshooter {

RandomPolar::RandomPolar(unsigned minPeak, unsigned maxPeak, unsigned HRHistoryLimit) :
    AbstractPolar(HRHistoryLimit),
    minPeak(minPeak),
    maxPeak(maxPeak)
{
}

void RandomPolar::setMinPeak(const unsigned &value) {
    minPeak = value;
}

void RandomPolar::setMaxPeak(const unsigned &value) {
    maxPeak = value;
}

unsigned RandomPolar::getInstantaneousHeartRate() {
    unsigned instantaneousHR = minPeak + (rand() % (maxPeak - minPeak + 1));

    addRecord(instantaneousHR);

    return instantaneousHR;
}

}
