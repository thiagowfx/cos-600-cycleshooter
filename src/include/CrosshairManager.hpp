#ifndef _CROSSHAIRMANAGER_HPP_
#define _CROSSHAIRMANAGER_HPP_

#include <OgreOverlay.h>
#include <OgreOverlayManager.h>

namespace Cycleshooter {
// TODO: sensibility and entropy
class CrosshairManager {
    Ogre::Overlay* crosshair;

    const double CROSSHAIR_SCALE = 0.25;

    inline void check_out_of_bounds(double& px, double &py, bool wraps);

public:
    CrosshairManager();

    void scroll(const double x, const double y, bool wraps = false);
    double getScrollX() const;
    double getScrollY() const;

    // setups
    void setupRunnerMode();
    void setupShooterMode();
};
}

#endif
