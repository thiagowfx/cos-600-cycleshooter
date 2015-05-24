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

    // context widgets
    trayManager->createLabel(OgreBites::TL_TOPLEFT, "contextLabel", "", 150);
    trayManager->createDecorWidget(OgreBites::TL_TOPLEFT, "contextLogo", "Cycleshooter/Running_man");

    // widgets available on both modes
    trayManager->createLabel(OgreBites::TL_RIGHT, "polarLabel", "HR: ", 125);
    trayManager->createLabel(OgreBites::TL_RIGHT, "ammoLabel", "Ammo: ", 125);

    // widgets available on runner mode only
    trayManager->createLabel(OgreBites::TL_BOTTOMRIGHT, "speedLabel", "Speed: ", 140);
    trayManager->createLabel(OgreBites::TL_BOTTOMRIGHT, "frictionLabel", "Friction: ", 140);

    // widgets available on shooter mode only
    // create any shooter-only widgets here

    // help panel
    Ogre::StringVector helpPanelItems = {"1", "2", "3"};
    Ogre::StringVector helpPanelValues = {"RunnerMode", "ShooterMode", "ToggleDebug"};
    trayManager->createParamsPanel(OgreBites::TL_NONE, "helpPanel", 160, helpPanelItems)->setAllParamValues(helpPanelValues);
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
    // update the trayManager
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
        // update widgets here
    }
}

void HUD::setupRunnerMode() {
    // update context mode
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("contextLabel"))->setCaption("Runner Mode");
    trayManager->destroyWidget("contextLogo");
    trayManager->createDecorWidget(OgreBites::TL_TOPLEFT, "contextLogo", "Cycleshooter/Running_man");

    // show/hide mode specific widgets
    trayManager->getTrayContainer(OgreBites::TL_BOTTOMRIGHT)->show();
    trayManager->getTrayContainer(OgreBites::TL_TOPRIGHT)->hide();
}

void HUD::setupShooterMode() {
    // update context mode
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("contextLabel"))->setCaption("Shooter Mode");
    trayManager->destroyWidget("contextLogo");
    trayManager->createDecorWidget(OgreBites::TL_TOPLEFT, "contextLogo", "Cycleshooter/Gun");

    // show/hide mode specific widgets
    trayManager->getTrayContainer(OgreBites::TL_BOTTOMRIGHT)->hide();
    trayManager->getTrayContainer(OgreBites::TL_TOPRIGHT)->show();
    /* TODO: either remove this (/\) if there are no shooter widgets, or create an if to be defensive
     * Otherwise, a small artifact appears on topright
     */
}

void HUD::setupDebugOn() {
    // fps
    trayManager->showFrameStats(OgreBites::TL_BOTTOMLEFT);

    // help panel
    trayManager->moveWidgetToTray("helpPanel", OgreBites::TL_BOTTOM, 0);
    trayManager->getWidget("helpPanel")->show();
}

void HUD::setupDebugOff() {
    // fps
    trayManager->hideFrameStats();

    // help panel
    trayManager->removeWidgetFromTray("helpPanel");
    trayManager->getWidget("helpPanel")->hide();
}

}
