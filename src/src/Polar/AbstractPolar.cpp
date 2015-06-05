#include "AbstractPolar.hpp"

namespace Cycleshooter {

void AbstractPolar::addRecord(const int &record) {
    if(HRHistory.size() == HRHistoryLimit)
        HRHistory.pop_front();
    HRHistory.push_back(record);
}

AbstractPolar::AbstractPolar(int HRHistoryLimit) :
    HRHistoryLimit(HRHistoryLimit)
{
}

int AbstractPolar::getMeanHeartRate() {
    if(HRHistory.empty())
        return getInstantaneousHeartRate();
    return std::accumulate(HRHistory.begin(), HRHistory.end(), 0) / HRHistory.size();
}

}
