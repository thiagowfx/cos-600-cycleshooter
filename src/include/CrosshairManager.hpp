#ifndef _CROSSHAIRMANAGER_HPP_
#define _CROSSHAIRMANAGER_HPP_

#include <OgreImage.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>

namespace Cycleshooter {

#define SQUARE(X) ((X)*(X))

class CrosshairManager {
    /**
     * The virtual crosshair overlay (fixed).
     */
    Ogre::Overlay* virtualCrosshair;

    /**
     * The real crosshair overlay (randomized around the fixed one).
     */
    Ogre::Overlay* realCrosshair;

    /**
     * Scale of the crosshair size.
     */
    const double CROSSHAIR_SCALE_SIZE = 1.0/4.0;

    /**
     * Scale of the crosshair sensibility.
     */
    const double VIRTUAL_CROSSHAIR_SENSIBILITY = 5e-3;

    /**
     * Return a random integer in the [-interval, +interval] range.
     */
    int getRandomIntegerOnRange(const int& interval) const {
        return (rand() % (2 * interval + 1)) - interval;
    }

public:
    CrosshairManager() {
        realCrosshair = Ogre::OverlayManager::getSingleton().getByName("Cycleshooter/RealCrosshair");
        realCrosshair->setScale(CROSSHAIR_SCALE_SIZE, CROSSHAIR_SCALE_SIZE);

        virtualCrosshair = Ogre::OverlayManager::getSingleton().getByName("Cycleshooter/VirtualCrosshair");
        virtualCrosshair->setScale(CROSSHAIR_SCALE_SIZE, CROSSHAIR_SCALE_SIZE);
    }

    /**
     * Move/scroll the crosshair by the amount specified. Optionally it may wrap around the screen too.
     */
    void scrollVirtualCrosshair(const int& heartRate, const double& dx, const double& dy, bool wraps = false) {
        printf("-- HR = %d --- (dx,dy) = (%lf, %lf)\n", heartRate, dx, dy);

        // double px = virtualCrosshair.first + VIRTUAL_CROSSHAIR_SENSIBILITY * SQUARE(heartRate) * dx;
        // double py = virtualCrosshair.second + VIRTUAL_CROSSHAIR_SENSIBILITY * SQUARE(heartRate) * dy;

        double px = virtualCrosshair->getScrollX() + dx;
        double py = virtualCrosshair->getScrollY() + dy;

        px = (px > 1.0) ? (wraps ? -1.0 : +1.0) : px;
        px = (px < -1.0) ? (wraps ? +1.0 : -1.0) : px;

        py = (py > 1.0) ? (wraps ? -1.0 : +1.0) : py;
        py = (py < -1.0) ? (wraps ? +1.0 : -1.0) : py;

        virtualCrosshair->setScroll(px, py);
        randomizeRealCrosshair(heartRate);
    }

    /**
     * Move the crosshair randomly each period of time in a square proportional to the heart beat.
     */
    void randomizeRealCrosshair(const double& heartRate) {
        int half_interval = 10;
        double dx = getRandomIntegerOnRange(half_interval) / 250.0;
        double dy = getRandomIntegerOnRange(half_interval) / 250.0;

        double px = virtualCrosshair->getScrollX() + (heartRate / 150.0) * dx;
        double py = virtualCrosshair->getScrollX() + (heartRate / 150.0) * dy;

        realCrosshair->setScroll(px, py);
    }

    /**
     * Return the x \in [-1.0, +1.0] coordinate of the crosshair.
     */
    double getRealCrosshairX() const {
        return realCrosshair->getScrollX();
    }

    /**
     * Return the y \in [-1.0, +1.0] coordinate of the crosshair.
     */
    double getRealCrosshairY() const {
        return realCrosshair->getScrollY();
    }

    /**
     * Map [-1.0, +1.0]^2 from crosshair coordinates to [0, width) x [0, height) image coordinates.
     */
    std::pair<int, int> convertToImageCoordinates(const Ogre::Image& image) const {
        auto width = image.getWidth();
        auto height = image.getHeight();

        // map [-1.0, +1.0] to [0, width)
        int retx = ((getRealCrosshairX() + 1.0) * width) / 2.0;

        // map [-1.0, +1.0] to [0, height)
        // inverted in the vertical
        int rety = height - int(((getRealCrosshairY() + 1.0) * height) / 2.0);

        if(retx == image.getWidth())
            --retx;

        if(rety == image.getHeight())
            --rety;

        return std::make_pair(retx, rety);
    }

    void setupRunnerMode() {
        realCrosshair->hide();
        virtualCrosshair->hide();
    }

    void setupShooterMode() {
        realCrosshair->show();
        virtualCrosshair->show();
    }

};

}

#endif
