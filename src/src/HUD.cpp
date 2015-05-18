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
    trayManager = new OgreBites::SdkTrayManager("TrayManager", controller->getRoot()->getAutoCreatedWindow(), controller->getInputContext(), controller->getBaseListener());
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
