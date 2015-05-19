#include "RandomPolar.hpp"

namespace Cycleshooter {

RandomPolar::RandomPolar(unsigned minPeak, unsigned maxPeak, unsigned historyLimit) :
    minPeak(minPeak),
    maxPeak(maxPeak),
    historyLimit(historyLimit)
{
    // TODO: move this upstream (and remove includes from the hpp)
    srand(time(NULL));
}

void RandomPolar::setMinHeartPeak(const unsigned &value) {
    minPeak = value;
}

void RandomPolar::setMaxHeartPeak(const unsigned &value) {
    maxPeak = value;
}

void RandomPolar::addRecord(const unsigned &record) {
    if(heartRateHistory.size() == historyLimit)
        heartRateHistory.pop_front();
    heartRateHistory.push_back(record);
}

int RandomPolar::getInstantaneousHeartRate(){
    // generate a random number
    unsigned instantaneousHeartRate = minPeak + (rand() % (maxPeak - minPeak));

    addRecord(instantaneousHeartRate);

    return instantaneousHeartRate;
}

int RandomPolar::getMeanHeartRate(){
    if(heartRateHistory.empty())
        return getInstantaneousHeartRate();

    unsigned sum = 0;

    for(auto& el : heartRateHistory) {
        sum += el;
    }

    return sum / heartRateHistory.size();
}

}
