#include "HUD.hpp"

namespace Cycleshooter {

Controller *HUD::getController() const {
    return controller;
}

void HUD::setController(Controller *value) {
    controller = value;
}

OgreBites::SdkTrayManager *HUD::getTrayManager() const {
    return trayManager;
}

void HUD::setTrayManager(OgreBites::SdkTrayManager *value) {
    trayManager = value;
}

void HUD::go() {
    Ogre::LogManager::getSingleton().logMessage("--> HUD: go <--");

    createTrayManager();
}

void HUD::createTrayManager() {
     Ogre::LogManager::getSingleton().logMessage("--> Creating Tray Manager <--");

    trayManager = new OgreBites::SdkTrayManager("TrayManager", controller->getWindow(), controller->getInputContext(), controller->getBaseListener());
    trayManager->hideCursor();
}

HUD::HUD(Controller *controller) :
    controller(controller)
{
    go();
}

HUD::~HUD() {
    if(trayManager)
        delete trayManager;
}

void HUD::setupRunnerMode() {
    trayManager->showLogo(OgreBites::TL_TOPLEFT);
}

void HUD::setupShooterMode() {
    trayManager->showLogo(OgreBites::TL_TOPRIGHT);
}

void HUD::setupNoneMode() {
    trayManager->hideLogo();
}

}
