#ifndef _RANDOMNESS_HPP_
#define _RANDOMNESS_HPP_

#include <cstdlib>
#include <ctime>

#include "Singleton.hpp"

namespace Cycleshooter {

class Randomness {
    SINGLETON(Randomness)

public:

    /**
     * Return a random integer in the [-interval, +interval] range.
     */
    static inline int getRandomIntegerOnRange(const int& interval) {
        return (rand() % (2 * interval + 1)) - interval;
    }

    /**
     * Return a double in the [0.0, 1.0] interval.
     */
    static inline double getRandomDouble() {
        return ((double)rand() / (double)RAND_MAX);
    }

    /**
     * Return either (+1) or (-1).
     */
    static inline int getRandomSignal() {
        return (rand() % 2) ? (+1) : (-1);
    }

};

}

#endif
