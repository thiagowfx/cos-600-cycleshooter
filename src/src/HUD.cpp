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

    trayManager->createLabel(OgreBites::TL_TOPLEFT, "contextLabel", "", 150);
    trayManager->createDecorWidget(OgreBites::TL_TOPLEFT, "contextLogo", "Cycleshooter/Running_man");

    trayManager->createLabel(OgreBites::TL_RIGHT, "polarLabel", "HR: ", 125);
    trayManager->createLabel(OgreBites::TL_RIGHT, "ammoLabel", "Ammo: ", 125);

    trayManager->createLabel(OgreBites::TL_BOTTOMRIGHT, "speedLabel", "Speed: ", 125);
    trayManager->createLabel(OgreBites::TL_BOTTOMRIGHT, "frictionLabel", "Friction: ", 125);
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

    // widgets updated on both modes
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("polarLabel"))->setCaption("HR: " + Ogre::StringConverter::toString(controller->getLogicManager()->getHeartRate()));
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("ammoLabel"))->setCaption("Ammo: " + Ogre::StringConverter::toString(controller->getLogicManager()->getAmmo()));

    // widgets updated on runner mode only
    if(controller->getContext() == CONTEXT_RUNNER) {
        dynamic_cast<OgreBites::Label*>(trayManager->getWidget("speedLabel"))->setCaption("TODO: speed");
        dynamic_cast<OgreBites::Label*>(trayManager->getWidget("frictionLabel"))->setCaption("TODO: friction");
    }

    // widgets updated on shooter mode only
    else if(controller->getContext() == CONTEXT_SHOOTER) {
    }
}

void HUD::setupRunnerMode() {
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("contextLabel"))->setCaption("Runner Mode");

    trayManager->destroyWidget("contextLogo");
    trayManager->createDecorWidget(OgreBites::TL_TOPLEFT, "contextLogo", "Cycleshooter/Running_man");

    trayManager->getTrayContainer(OgreBites::TL_BOTTOMRIGHT)->show();
}

void HUD::setupShooterMode() {
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("contextLabel"))->setCaption("Shooter Mode");

    trayManager->destroyWidget("contextLogo");
    trayManager->createDecorWidget(OgreBites::TL_TOPLEFT, "contextLogo", "Cycleshooter/Gun");

    trayManager->getTrayContainer(OgreBites::TL_BOTTOMRIGHT)->hide();
}

void HUD::setupDebugOn() {
    trayManager->showFrameStats(OgreBites::TL_BOTTOMLEFT);
}

void HUD::setupDebugOff() {
    trayManager->hideFrameStats();
}

}
