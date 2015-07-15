#ifndef _CONSTANTBICYCLE_HPP_
#define _CONSTANTBICYCLE_HPP_

#include "AbstractBicycle.hpp"

namespace Cycleshooter {
/**
 * @brief Simulates a bicycle with a fixed amount of
 * speed, which can be increased or decreased manually over time.
 */
class ConstantBicycle : public AbstractBicycle {

public:
    ConstantBicycle(const int& speed) :
        AbstractBicycle(speed)
    {
        setFriction(0);
    }

    virtual void updateSpeed() {
        updateStatistics(speed);
    }

    virtual void setFriction(const int& value) {
        friction = value;
    }

    virtual void changeSpeed(const int& amount) {
        speed = std::max(0, speed + amount);
    }
};

}

#endif
