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
    createTrayWidgets();
}

void HUD::createTrayManager() {
    Ogre::LogManager::getSingleton().logMessage("--> Creating Tray Manager <--");

    trayManager = new OgreBites::SdkTrayManager("trayManager", controller->getWindow(), *inputContext);
    trayManager->hideCursor();
}

void HUD::createTrayWidgets() {
    Ogre::LogManager::getSingleton().logMessage("--> Creating Tray Widgets <--");

    polarLabel = trayManager->createLabel(OgreBites::TL_LEFT, "polarLabel", "HR: ", 140);
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
    polarLabel->setCaption("HR : " + Ogre::StringConverter::toString(controller->getPolar()->getInstantaneousHeartRate()));
}

void HUD::setupRunnerMode() {
    if(contextWidget)
        trayManager->destroyWidget(contextWidget);
    contextWidget = trayManager->createLabel(OgreBites::TL_TOPRIGHT, "contextWidget", "Runner Mode", 170);
}

void HUD::setupShooterMode() {
    if(contextWidget)
        trayManager->destroyWidget(contextWidget);
    contextWidget = trayManager->createLabel(OgreBites::TL_TOPRIGHT, "contextWidget", "Shooter Mode", 170);
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
