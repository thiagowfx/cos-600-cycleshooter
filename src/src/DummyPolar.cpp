#include "DummyPolar.hpp"

namespace Cycleshooter {

DummyPolar::DummyPolar() {
    // Initialize the size of the vector to get the mean value
    itrMeanHeartRate = 200;
    // Clear the vector (Guarantee the vector initialize empty)
    recordHeartRate.clear();
}

DummyPolar::~DummyPolar() {
    // Clear the vector
    recordHeartRate.clear();
}

void DummyPolar::setupHeartPeaks(short min, short max){
    minHeartPeak = min;
    maxHeartPeak = max;
}

short DummyPolar::readInstantaneousHeartRate(){

    short heartRate;

    // Initialize random seed
    srand (time(NULL));

    // Generate a random number
    heartRate = rand() % (maxHeartPeak-minHeartPeak) + minHeartPeak;

    // Save the value
    if(recordHeartRate.size() < itrMeanHeartRate){
        recordHeartRate.push_back(heartRate);
    } else {
        recordHeartRate.erase(recordHeartRate.begin() + 1);
        recordHeartRate.push_back(heartRate);
    }

    return heartRate;
}

short DummyPolar::readMeanHeartRate(){
    int sum = 0;

    if (recordHeartRate.empty()){
        // If the vector is empty, return the instantaneous value
        return readInstantaneousHeartRate();

    } else {
        // Calculate the sum of all elements in the vector
        for(std::vector<int>::iterator it=recordHeartRate.begin();
            it!=recordHeartRate.end();++it)
            sum += *it;

        // return the mean of the vector
        return (sum/recordHeartRate.size());

    }
}
}
