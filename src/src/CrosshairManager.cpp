#include "CrosshairManager.hpp"

namespace Cycleshooter {

Ogre::Overlay *CrosshairManager::getCrosshair() const {
    return crosshair;
}

CrosshairManager::CrosshairManager() {
    crosshair = Ogre::OverlayManager::getSingleton().getByName("Cycleshooter/Crosshair");
    crosshair->setScale(0.30, 0.30);
}

CrosshairManager::~CrosshairManager() {
    Ogre::OverlayManager::getSingleton().destroy("Cycleshooter/Crosshair");
}

void CrosshairManager::setupRunnerMode() {
    crosshair->hide();
}

void CrosshairManager::setupShooterMode() {
    crosshair->show();
}

}
