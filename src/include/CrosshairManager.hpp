#ifndef _CROSSHAIRMANAGER_HPP_
#define _CROSSHAIRMANAGER_HPP_

#include <OgreImage.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>

namespace Cycleshooter {

#define SQUARE(X) ((X)*(X))

class CrosshairManager {

    /**
     * The crosshair overlay.
     */
    Ogre::Overlay* crosshair;

    /**
     * Virtual crosshair.
     */
    std::pair<double, double> virtualCrosshair;

    /**
     * Reduction scale of the crosshair size.
     */
    const double CROSSHAIR_SCALE_SIZE = 0.25;

    /**
     * Scale of the crosshair sensibility.
     */
    const double CROSSHAIR_SENSIBILITY = 5e-3;

    /**
     * Return a random integer in the [-interval, +interval] range.
     */
    int getRandomIntRange(int interval) const {
        return (rand() % (2 * interval + 1)) - interval;
    }

public:
    CrosshairManager() {
        crosshair = Ogre::OverlayManager::getSingleton().getByName("Cycleshooter/Crosshair");
        crosshair->setScale(CROSSHAIR_SCALE_SIZE, CROSSHAIR_SCALE_SIZE);

        setVirtualCrosshair(std::pair<double,double>(crosshair->getScrollX(), crosshair->getScrollY()));
    }

    /**
     * Move/scroll the crosshair by the amount specified. Optionally it may wrap around the screen too.
     */
    void scroll(const double heartRate, const double& dx, const double& dy, bool wraps = false) {
        double px = virtualCrosshair.first + CROSSHAIR_SENSIBILITY * SQUARE(heartRate) * dx;
        double py = virtualCrosshair.second + CROSSHAIR_SENSIBILITY * SQUARE(heartRate) * dy;

        px = (px > 1.0) ? (wraps ? -1.0 : +1.0) : px;
        px = (px < -1.0) ? (wraps ? +1.0 : -1.0) : px;

        py = (py > 1.0) ? (wraps ? -1.0 : +1.0) : py;
        py = (py < -1.0) ? (wraps ? +1.0 : -1.0) : py;

        setVirtualCrosshair(std::pair<double, double>(px, py));
    }

    /**
     * Move the crosshair randomly each period of time in a square proportional to the heart beat.
     */
    void randomizeCrosshair(const double& heartRate) {
        int half_interval = 10;
        double dx = getRandomIntRange(half_interval) / 250.0;
        double dy = getRandomIntRange(half_interval) / 250.0;

        double px = virtualCrosshair.first + (heartRate / 150.0) * dx;
        double py = virtualCrosshair.second + (heartRate / 150.0) * dy;

        crosshair->setScroll(px, py);
    }

    /**
     * Return the (x,y) \in [-1.0, +1.0]^2 coordinates of the crosshair.
     */
    std::pair<double, double> getScrollXY() const {
        return std::make_pair(getScrollX(), getScrollY());
    }

    /**
     * Return the x \in [-1.0, +1.0] coordinate of the crosshair.
     */
    double getScrollX() const {
        return crosshair->getScrollX();
    }

    /**
     * Return the y \in [-1.0, +1.0] coordinate of the crosshair.
     */
    double getScrollY() const {
        return crosshair->getScrollY();
    }

    /**
     * Map [-1.0, +1.0]^2 from crosshair coordinates to [0, width) x [0, height) image coordinates.
     */
    std::pair<int, int> convertToImageCoordinates(const Ogre::Image& image) const {
        auto width = image.getWidth();
        auto height = image.getHeight();

        // map [-1.0, +1.0] to [0, width)
        int retx = ((getScrollX() + 1.0) * width) / 2.0;

        // map [-1.0, +1.0] to [0, height)
        // inverted in the vertical
        int rety = height - int(((getScrollY() + 1.0) * height) / 2.0);

        if(retx == image.getWidth())
            --retx;

        if(rety == image.getHeight())
            --rety;

        return std::make_pair(retx, rety);
    }

    void setupRunnerMode() {
        crosshair->hide();
    }

    void setupShooterMode() {
        crosshair->show();
    }

    void setVirtualCrosshair(const std::pair<double, double> &value) {
        virtualCrosshair = value;
    }

    std::pair<double, double> getVirtualCrosshair() const {
        return virtualCrosshair;
    }

};

}

#endif
