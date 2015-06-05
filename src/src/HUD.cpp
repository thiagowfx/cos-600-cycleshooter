#include "HUD.hpp"

namespace Cycleshooter {

void HUD::go() {
    Ogre::LogManager::getSingleton().logMessage("--> HUD: go <--");

    createTrayManager();
    createTrayWidgets();
}

void HUD::createTrayManager() {
    Ogre::LogManager::getSingleton().logMessage("--> HUD: Creating Tray Manager <--");

    trayManager = std::unique_ptr<OgreBites::SdkTrayManager>(new OgreBites::SdkTrayManager("trayManager", controller->getWindow(), OgreBites::InputContext()));
    trayManager->hideCursor();
}

void HUD::createTrayWidgets() {
    Ogre::LogManager::getSingleton().logMessage("--> HUD: Creating Tray Widgets <--");

    // context widgets
    trayManager->createLabel(CONTEXT_TL, "contextLabel", "", 150);
    trayManager->createDecorWidget(CONTEXT_TL, "contextLogo", "Cycleshooter/Running_man");

    // widgets available on both modes
    trayManager->createLabel(BOTH_MODES_TL, "polarLabel", "HR: ", 125);
    trayManager->createLabel(BOTH_MODES_TL, "ammoLabel", "Ammo: ", 125);

    // widgets available on runner mode only
    trayManager->createLabel(RUNNERMODE_TL, "speedLabel", "Speed: ", 140);
    trayManager->createLabel(RUNNERMODE_TL, "frictionLabel", "Friction: ", 140);
}

void HUD::setHelpPanel(Ogre::StringVector params, Ogre::StringVector values) {
    if(trayManager->getWidget("helpPanel")) {
        trayManager->destroyWidget("helpPanel");
    }
    trayManager->createParamsPanel(HELP_PANEL_TL, "helpPanel", 160, params)->setAllParamValues(values);
}

HUD::HUD(Controller *controller) :
    controller(controller)
{
    go();
}

void HUD::update(const Ogre::FrameEvent& evt) {
    // update the trayManager
    trayManager->frameRenderingQueued(evt);

    // widgets updated on both modes
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("polarLabel"))->setCaption("HR: " + Ogre::StringConverter::toString(controller->getLogicManager()->getPlayerHeartRate()));
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("ammoLabel"))->setCaption("Ammo: " + Ogre::StringConverter::toString(controller->getLogicManager()->getPlayerAmmo()));

    // widgets updated on runner mode only
    if(controller->getContext() == CONTEXT_RUNNER) {
        dynamic_cast<OgreBites::Label*>(trayManager->getWidget("speedLabel"))->setCaption("TODO: speed");
        dynamic_cast<OgreBites::Label*>(trayManager->getWidget("frictionLabel"))->setCaption("TODO: friction");
    }
}

void HUD::setupRunnerMode() {
    // update context mode
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("contextLabel"))->setCaption("Runner Mode");
    trayManager->destroyWidget("contextLogo");
    trayManager->createDecorWidget(CONTEXT_TL, "contextLogo", "Cycleshooter/Running_man");

    // show/hide mode specific widgets
    trayManager->getTrayContainer(RUNNERMODE_TL)->show();
}

void HUD::setupShooterMode() {
    // update context mode
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("contextLabel"))->setCaption("Shooter Mode");
    trayManager->destroyWidget("contextLogo");
    trayManager->createDecorWidget(CONTEXT_TL, "contextLogo", "Cycleshooter/Gun");

    // show/hide mode specific widgets
    trayManager->getTrayContainer(RUNNERMODE_TL)->hide();
}

void HUD::setupDebugOn() {
    // fps
    trayManager->showFrameStats(FPS_TL);

    // help panel
    if(trayManager->getWidget("helpPanel")) {
        trayManager->moveWidgetToTray("helpPanel", HELP_PANEL_TL, 0);
        trayManager->getWidget("helpPanel")->show();
    }
}

void HUD::setupDebugOff() {
    // fps
    trayManager->hideFrameStats();

    // help panel
    if(trayManager->getWidget("helpPanel")) {
        trayManager->removeWidgetFromTray("helpPanel");
        trayManager->getWidget("helpPanel")->hide();
    }
}

}
