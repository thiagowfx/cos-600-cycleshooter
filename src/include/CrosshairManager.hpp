#ifndef _CROSSHAIRMANAGER_HPP_
#define _CROSSHAIRMANAGER_HPP_

#include "OgreOverlay.h"
#include "OgreOverlayManager.h"

namespace Cycleshooter {
class CrosshairManager {
    Ogre::Overlay* crosshair;

public:
    CrosshairManager();
    virtual ~CrosshairManager();

    // setups
    void setupRunnerMode();
    void setupShooterMode();

    // getter
    Ogre::Overlay *getCrosshair() const;
};
}

#endif
