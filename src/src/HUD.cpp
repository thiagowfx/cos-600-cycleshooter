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

    // information widgets
    int INFO_SIZE = 140;
    trayManager->createLabel(INFO_WIDGETS_TL, "polarLabel", "HR: ", INFO_SIZE );
    trayManager->createLabel(INFO_WIDGETS_TL, "speedLabel", "Speed: ", INFO_SIZE );
    trayManager->createLabel(INFO_WIDGETS_TL, "loadLabel", "Speed: ", INFO_SIZE );
    trayManager->createLabel(INFO_WIDGETS_TL, "ammoLabel", "Ammo: ", INFO_SIZE );
    trayManager->createLabel(INFO_WIDGETS_TL, "monsterLabel", "Monster: ", INFO_SIZE );
}

HUD::HUD(Controller *controller) :
    controller(controller)
{
    go();
}

void HUD::update(const Ogre::FrameEvent& evt) {
    // update the trayManager
    trayManager->frameRenderingQueued(evt);

    // update information widgets
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("polarLabel"))->setCaption("HR: " + Ogre::StringConverter::toString(controller->getLogicManager()->getPlayerHeartRate()));
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("speedLabel"))->setCaption("Speed: TODO");
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("loadLabel"))->setCaption("Load: TODO");
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("ammoLabel"))->setCaption("Ammo: " + Ogre::StringConverter::toString(controller->getLogicManager()->getPlayerAmmo()));
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("monsterLabel"))->setCaption("Monster: " + Ogre::StringConverter::toString(controller->getLogicManager()->getMonsterHealth()));
}

void HUD::setupRunnerMode() {
    // update context mode
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("contextLabel"))->setCaption("Runner Mode");
    trayManager->destroyWidget("contextLogo");
    trayManager->createDecorWidget(CONTEXT_TL, "contextLogo", "Cycleshooter/Running_man");
}

void HUD::setupShooterMode() {
    // update context mode
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("contextLabel"))->setCaption("Shooter Mode");
    trayManager->destroyWidget("contextLogo");
    trayManager->createDecorWidget(CONTEXT_TL, "contextLogo", "Cycleshooter/Gun");
}

void HUD::setupDebugOn() {
    // fps
    trayManager->showFrameStats(FPS_TL);
}

void HUD::setupDebugOff() {
    // fps
    trayManager->hideFrameStats();
}

}
