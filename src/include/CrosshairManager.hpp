#ifndef _CROSSHAIRMANAGER_HPP_
#define _CROSSHAIRMANAGER_HPP_

#include <OgreImage.h>
#include <OgreOverlayManager.h>

#include "ConfigManager.hpp"
#include "Randomness.hpp"

namespace Cycleshooter {

class CrosshairManager {
    /**
     * The green crosshair overlay (fixed).
     */
    Ogre::Overlay* greenCrosshair;

    /**
     * The red crosshair overlay (randomized around the fixed one).
     */
    Ogre::Overlay* redCrosshair;

    /**
     * Scale of the crosshair size.
     */
    const double CROSSHAIR_SCALE_SIZE = ConfigManager::instance().getDouble("CrosshairManager.crosshair_scale_size");

    /**
     * Crosshair anti-sensibility. More means less sensible.
     */
    const double GREEN_CROSSHAIR_FRICTION = ConfigManager::instance().getDouble("CrosshairManager.green_crosshair_friction");

    /**
     * Probability for the red crosshair to change its direction in a randomization.
     */
    const double PROBABILITY_CHANGE_DIRECTION = ConfigManager::instance().getDouble("CrosshairManager.probability_change_direction");

    /**
     * Randomization anti-sensibility. Less means a more aggressive randomization.
     */
    const double RANDOMIZATION_FRICTION = ConfigManager::instance().getDouble("CrosshairManager.randomization_friction_relative") * GREEN_CROSSHAIR_FRICTION;

    /**
     * Size of the square of randomization of the red crosshair around the green crosshair.
     */
    const double RANDOMIZATION_SQUARE_HALF_SIDE = ConfigManager::instance().getDouble("CrosshairManager.randomization_square_half_side_relative") * (1.0 / GREEN_CROSSHAIR_FRICTION);

    const int HEARTBEAT_MINIMUM_ASSUMED = ConfigManager::instance().getInt("Controller.heartbeat_minimum_assumed");
    const int HEARTBEAT_MAXIMUM_ASSUMED = ConfigManager::instance().getInt("Controller.heartbeat_maximum_assumed");
    const double MAPPED_HEARTBEAT_MINIMUM = ConfigManager::instance().getDouble("CrosshairManager.mapped_heartbeat_minimum");
    const double MAPPED_HEARTBEAT_MAXIMUM = ConfigManager::instance().getDouble("CrosshairManager.mapped_heartbeat_maximum");

    inline std::pair<double, double> getRandomDirection() const {
        double a = Randomness::instance().getRandomSignal() * Randomness::instance().getRandomDouble();
        double b = Randomness::instance().getRandomSignal() * Randomness::instance().getRandomDouble();
        double to_norm = sqrt(a * a + b * b);
        return std::make_pair(a / to_norm, b / to_norm);
    }

    /**
     * Function to determine the scroll amount, based on the current heartRate.
     */
    inline double fHeartRateSensibility(const double& p, const int& heartRate) const {
        // map heartRate to mappedHeartRate
        double mappedHeartRate = (((MAPPED_HEARTBEAT_MAXIMUM - MAPPED_HEARTBEAT_MINIMUM) * (heartRate - HEARTBEAT_MINIMUM_ASSUMED)) / (HEARTBEAT_MAXIMUM_ASSUMED - HEARTBEAT_MINIMUM_ASSUMED)) + MAPPED_HEARTBEAT_MINIMUM;
        return (p / GREEN_CROSSHAIR_FRICTION) * mappedHeartRate;
    }

public:
    CrosshairManager() {
        redCrosshair = Ogre::OverlayManager::getSingleton().getByName("Cycleshooter/RedCrosshair");
        redCrosshair->setScale(CROSSHAIR_SCALE_SIZE, CROSSHAIR_SCALE_SIZE);

        greenCrosshair = Ogre::OverlayManager::getSingleton().getByName("Cycleshooter/GreenCrosshair");
        greenCrosshair->setScale(CROSSHAIR_SCALE_SIZE, CROSSHAIR_SCALE_SIZE);
    }

    /**
     * Move/scroll the crosshair by the amount specified. Optionally it may wrap around the screen too.
     */
    void scrollVirtualCrosshair(const int& heartRate, const double& dx, const double& dy, bool wraps = false) {
        double px = greenCrosshair->getScrollX() + fHeartRateSensibility(dx, heartRate);
        double py = greenCrosshair->getScrollY() + fHeartRateSensibility(dy, heartRate);

        // handle out-of-screen crosshair coordinates appropriately
        px = (px > 1.0) ? (wraps ? -1.0 : +1.0) : px, px = (px < -1.0) ? (wraps ? +1.0 : -1.0) : px;
        py = (py > 1.0) ? (wraps ? -1.0 : +1.0) : py, py = (py < -1.0) ? (wraps ? +1.0 : -1.0) : py;

        greenCrosshair->setScroll(px, py);
        redCrosshair->setScroll(px, py);
    }

    /**
     * Move the crosshair along a direction in each period of time.
     * The direction occasionally changes.
     */
    void randomizeRedCrosshair() {
        static std::pair<double, double> direction = getRandomDirection();

        double dx = direction.first / RANDOMIZATION_FRICTION;
        double dy = direction.second / RANDOMIZATION_FRICTION;

        double px = redCrosshair->getScrollX() + dx;
        double py = redCrosshair->getScrollY() + dy;

        // check if px is out-of-square
        if (fabs(px - greenCrosshair->getScrollX()) > RANDOMIZATION_SQUARE_HALF_SIDE) {
            px -= 2 * dx;
            direction.first = -direction.first;
        }

        // check if py is out-of-square
        if (fabs(py - greenCrosshair->getScrollY()) > RANDOMIZATION_SQUARE_HALF_SIDE) {
            py -= 2 * dy;
            direction.second = -direction.second;
        }

        // handle change of direction
        double P = Randomness::instance().getRandomDouble();
        if (P >= (1 - PROBABILITY_CHANGE_DIRECTION)) {
            direction = getRandomDirection();
        }

        redCrosshair->setScroll(px, py);
    }

    /**
     * Return the x \in [-1.0, +1.0] coordinate of the red crosshair.
     */
    double getRedCrosshairX() const {
        return redCrosshair->getScrollX();
    }

    /**
     * Return the y \in [-1.0, +1.0] coordinate of the red crosshair.
     */
    double getRedCrosshairY() const {
        return redCrosshair->getScrollY();
    }

    /**
     * Map [-1.0, +1.0]^2 from (red) crosshair coordinates to [0, width) x [0, height) image coordinates.
     */
    std::pair<int, int> convertToImageCoordinates(const Ogre::Image& image) const {
        auto width = image.getWidth();
        auto height = image.getHeight();

        // map [-1.0, +1.0] to [0, width)
        int retx = ((getRedCrosshairX() + 1.0) * width) / 2.0;

        // map [-1.0, +1.0] to [0, height)
        // inverted in the vertical
        int rety = height - int(((getRedCrosshairY() + 1.0) * height) / 2.0);

        if(retx == image.getWidth())
            --retx;

        if(rety == image.getHeight())
            --rety;

        return std::make_pair(retx, rety);
    }

    void setupRunnerMode() {
        redCrosshair->hide();
        greenCrosshair->hide();
    }

    void setupShooterMode() {
        redCrosshair->show();
        greenCrosshair->show();
    }

};

}

#endif
