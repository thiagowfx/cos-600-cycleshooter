#ifndef _CROSSHAIRMANAGER_HPP_
#define _CROSSHAIRMANAGER_HPP_

#include <OgreImage.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>

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
    const double CROSSHAIR_SCALE_SIZE = 1.0/5.0;

    /**
     * Scale of the crosshair sensibility.
     */
    const double GREEN_CROSSHAIR_SENSIBILITY = 5e-3;

    /**
     * Return a random integer in the [-interval, +interval] range.
     */
    int getRandomIntegerOnRange(const int& interval) const {
        return (rand() % (2 * interval + 1)) - interval;
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
        printf("-- HR = %d --- (dx,dy) = (%lf, %lf)\n", heartRate, dx, dy);

        // double px = greenCrosshair.first + GREEN_CROSSHAIR_SENSIBILITY * heartRate * dx;
        // double py = greenCrosshair.second + GREEN_CROSSHAIR_SENSIBILITY * heartRate * dy;

        double px = greenCrosshair->getScrollX() + dx / 25.0;
        double py = greenCrosshair->getScrollY() + dy / 25.0;

        px = (px > 1.0) ? (wraps ? -1.0 : +1.0) : px;
        px = (px < -1.0) ? (wraps ? +1.0 : -1.0) : px;

        py = (py > 1.0) ? (wraps ? -1.0 : +1.0) : py;
        py = (py < -1.0) ? (wraps ? +1.0 : -1.0) : py;

        greenCrosshair->setScroll(px, py);
        randomizeRedCrosshair(heartRate);
    }

    /**
     * Move the crosshair randomly each period of time in a square proportional to the heart beat.
     */
    void randomizeRedCrosshair(const int& heartRate) {
        int half_interval = 10;
        double dx = getRandomIntegerOnRange(half_interval) / 250.0;
        double dy = getRandomIntegerOnRange(half_interval) / 250.0;

        double px = greenCrosshair->getScrollX() + (heartRate / 150.0) * dx;
        double py = greenCrosshair->getScrollX() + (heartRate / 150.0) * dy;

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
