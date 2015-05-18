#include "HUD.hpp"

namespace Cycleshooter {

Controller *HUD::getController() const {
    return controller;
}

OgreBites::SdkTrayManager *HUD::getTrayManager() const {
    return trayManager;
}

void HUD::go() {
    Ogre::LogManager::getSingleton().logMessage("--> HUD: go <--");
    createTrayManager();
}

void HUD::createTrayManager() {
    Ogre::LogManager::getSingleton().logMessage("--> Creating Tray Manager <--");

    trayManager = new OgreBites::SdkTrayManager("TrayManager", controller->getWindow(), *inputContext);
    trayManager->hideCursor();
}

HUD::HUD(Controller *controller, OgreBites::InputContext *inputContext) :
    controller(controller),
    inputContext(inputContext)
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
