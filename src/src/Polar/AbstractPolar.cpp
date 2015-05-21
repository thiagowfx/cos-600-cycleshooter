#include "AbstractPolar.hpp"

namespace Cycleshooter {

void AbstractPolar::addRecord(const unsigned &record) {
    if(HRHistory.size() == HRHistoryLimit)
        HRHistory.pop_front();
    HRHistory.push_back(record);
}

AbstractPolar::AbstractPolar(unsigned HRHistoryLimit) :
    HRHistoryLimit(HRHistoryLimit)
{
}

unsigned AbstractPolar::getMeanHeartRate() {
    if(HRHistory.empty())
        return getInstantaneousHeartRate();
    return std::accumulate(HRHistory.begin(), HRHistory.end(), 0) / HRHistory.size();
}

}
