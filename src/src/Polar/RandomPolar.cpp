#include "RandomPolar.hpp"

namespace Cycleshooter {

RandomPolar::RandomPolar(int minPeak, int maxPeak) :
    AbstractPolar(),
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

    // Statistics variables
    updateStatistics(heartRate);

    return heartRate;
}

}
