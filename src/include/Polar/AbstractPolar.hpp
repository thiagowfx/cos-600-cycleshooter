#ifndef _ABSTRACTPOLAR_HPP_
#define _ABSTRACTPOLAR_HPP_

#include <algorithm>
#include <deque>

namespace Cycleshooter {
class AbstractPolar {
protected:
    /**
     * Record/History of latest heart rates.
     */
    std::deque<int> HRHistory;

    /**
     * Maximum number of entries in heart rate history.
     */
    const int HRHistoryLimit;

    /**
     * Add a new record to the history of heart rates.
     */
    void addRecord(const int& record);

public:
    AbstractPolar(int HRHistoryLimit = 120);

    /**
     * Get a single value to represent the heart rate and record it.
     */
    virtual int getInstantaneousHeartRate() = 0;

    /**
     * Get the mean of the recorded heart rates.
     * Useful to get the base heart rate in the beginning of the game.
     */
    int getMeanHeartRate();
};

}

#endif
