#include "AbstractPolar.hpp"

namespace Cycleshooter {

AbstractPolar::AbstractPolar()
{
}

std::string AbstractPolar::printStatistics() {
    std::cout << "Number of records acquired in the game: "<< count << std::endl;
    std::cout << "Median of Heart beats: " << (sum/count) << std::endl;
    std::cout << "Greatest record of Heart beat: " << max << std::endl;
    std::cout << "Lowest record of Heart beat: " << min << std::endl;
}

void AbstractPolar::updateStatistics(int heartRate){
    // Statistics variables
    if (count == 0){
        max = heartRate;
        min = heartRate;

    } else {
        if (heartRate > max)
            max = heartRate;

        if (heartRate < min)
            min = heartRate;
    }

    // Updating variables
    sum += heartRate;
    count++;

}

}
