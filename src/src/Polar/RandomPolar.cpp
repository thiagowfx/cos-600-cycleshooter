#include "RandomPolar.hpp"

namespace Cycleshooter {

RandomPolar::RandomPolar(int minPeak, int maxPeak, int HRHistoryLimit) :
    AbstractPolar(HRHistoryLimit),
    minPeak(minPeak),
    maxPeak(maxPeak)
{
}

void RandomPolar::setMinPeak(const int &value) {
    minPeak = value;
}

void RandomPolar::setMaxPeak(const int &value) {
    maxPeak = value;
}

int RandomPolar::getInstantaneousHeartRate() {
    int heartRate = minPeak + (rand() % (maxPeak - minPeak + 1));
    addRecord(heartRate);
    return heartRate;
}

}
