#include "HUD.hpp"

namespace Cycleshooter {

void HUD::go() {
    Ogre::LogManager::getSingleton().logMessage("--> HUD: go <--");

    createTrayManager();
    createTrayWidgets();
}

void HUD::createTrayManager() {
    Ogre::LogManager::getSingleton().logMessage("--> HUD: Creating Tray Manager <--");

    trayManager = new OgreBites::SdkTrayManager("trayManager", controller->getWindow(), *inputContext);
    trayManager->hideCursor();
}

void HUD::createTrayWidgets() {
    Ogre::LogManager::getSingleton().logMessage("--> HUD: Creating Tray Widgets <--");

    trayManager->createLabel(OgreBites::TL_TOPRIGHT, "contextLabel", "", 170);
    trayManager->createLabel(OgreBites::TL_LEFT, "polarLabel", "HR: ", 125);
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

void HUD::update(const Ogre::FrameEvent& evt) {
    trayManager->frameRenderingQueued(evt);
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("polarLabel"))->setCaption("HR: " + Ogre::StringConverter::toString(controller->getLogicManager()->getHeartRate()));
}

void HUD::setupRunnerMode() {
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("contextLabel"))->setCaption("Runner Mode");
}

void HUD::setupShooterMode() {
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("contextLabel"))->setCaption("Shooter Mode");
}

void HUD::setupDebugOn() {
    // OGRE Logo
    trayManager->showLogo(OgreBites::TL_TOPLEFT);

    // FPS
    trayManager->showFrameStats(OgreBites::TL_BOTTOMLEFT);
}

void HUD::setupDebugOff() {
    trayManager->hideLogo();
    trayManager->hideFrameStats();
}

}
