#ifndef _ABSTRACTBICYCLE_HPP_
#define _ABSTRACTBICYCLE_HPP_

#include <limits>

#include "ConfigManager.hpp"

namespace Cycleshooter {

class AbstractBicycle {

    struct {
        /**
         * Sum of the values acquired in this session.
         */
        long long int sum = 0;

        /**
         * Number of the values acquired in this session.
         */
        long long int count = 0;

        /**
         * Maximum value of the velocity in this session.
         */
        int greatest = std::numeric_limits<int>::min();
    } stats;

    /**
     * Factor to convert from the real speed (RPM) to the logical speed (units of the game).
     */
    const double RPM_TO_LOGICAL_SPEED = ConfigManager::instance().getDouble("Bicycle.rpm_to_logical_speed");

protected:
    /**
     * Friction (also difficulty, load): resistance of the bicycle to movement.
     * Tipically in the 0--400 range.
     */
    int friction;

    /**
     * Current speed of the bicycle (RPM). Tipically in the 0--100 range.
     */
    int speed;

    /**
     * Update the statistics about the Bicycle.
     */
    void updateStatistics(const int& speed) {
        this->speed = speed;

        if(speed > 0) {
            stats.greatest = std::max(stats.greatest, speed);
            stats.sum += speed;
            ++stats.count;
        }
    }

    /**
     * The maximum load/friction we can set the bicycle to.
     */
    const int MAXIMUM_LOAD = ConfigManager::instance().getInt("RealBicycle.maximum_load");

public:
    AbstractBicycle(){}

    AbstractBicycle(const int& speed) :
        speed(speed)
    {}

    /**
     * Print the statistics about this session.
     */
    void printStatistics(std::ostream& os = std::cout) const {
        os << "* Bicycle Statistics\n"
              "- # of records acquired: "<< stats.count << std::endl <<
              "- Greatest speed: " << stats.greatest << std::endl <<
              "- Mean: " << (static_cast<double>(stats.sum) / stats.count) << std::endl;
    }

    /**
     * Get the current speed of the bicycle (in RPM, the real speed provided by the bicycle, without any conversion).
     */
    int getRpmSpeed() const {
        return speed;
    }

    /**
     * Get the current speed of the bicycle (in speed units of the game).
     */
    double getGameSpeed() const {
        return RPM_TO_LOGICAL_SPEED * speed;
    }

    /**
     * Update the speed of the bicycle, by reading a response from its controller circuit.
     */
    virtual void updateSpeed() = 0;

    /**
     * Get the current friction of the bicycle.
     */
    int getFriction() const {
        return friction;
    }

    /**
     * Set the friction of the bicycle, by sending a command to the controller circuit.
     */
    virtual void setFriction(const int& value) = 0;

    /**
     * Changes (increments or decrements) the friction of the bicycle by the specified value.
     */
    virtual void changeFriction(const int& value) {
        friction = std::max(0, friction + value);
        friction = std::min(MAXIMUM_LOAD, friction);
        setFriction(friction);
    }

    /**
     * Changes (increments or decrements) the speed by the specified value.
     */
    virtual void changeSpeed(const int& amount) {}
};

}

#endif
