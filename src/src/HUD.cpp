#include "HUD.hpp"

namespace Cycleshooter {

void HUD::createTrayManager() {
    LOG("HUD: Creating Tray Manager");

    trayManager = std::unique_ptr<OgreBites::SdkTrayManager>(new OgreBites::SdkTrayManager("trayManager", controller->getWindow()));
    trayManager->hideCursor();
}

void HUD::createTrayWidgets() {
    LOG("HUD: Creating Tray Widgets");

    // context widgets
    trayManager->createLabel(CONTEXT_TL, "contextLabel", "", 150);
    trayManager->createDecorWidget(CONTEXT_TL, "contextLogo", "Cycleshooter/RunningModePanel");

    int INFO_PANEL_SIZE = 170;
    Ogre::StringVector infoParams = {"HeartRate", "Speed(rpm)", "Friction"};
    Ogre::StringVector infoValues = Ogre::StringVector(infoParams.size(), "");
    trayManager->createParamsPanel(INFO_WIDGETS_TL, "infoPanel", INFO_PANEL_SIZE, infoParams)->setAllParamValues(infoValues);

    int AMMO_PANEL_SIZE = 240;
    Ogre::StringVector ammoParams = {"Monster Health", "Ammo", "Ammo in track", "Ammo refreshes in"};
    Ogre::StringVector ammoValues = Ogre::StringVector(ammoParams.size(), "");
    trayManager->createParamsPanel(AMMO_WIDGETS_TL, "ammoPanel", AMMO_PANEL_SIZE, ammoParams)->setAllParamValues(ammoValues);

    const int CLOCK_SIZE = 170;
    trayManager->createLabel(CLOCK_TL, "clockLabel", "", CLOCK_SIZE);
}

void HUD::update(const Ogre::FrameEvent& evt) {
    // update the trayManager
    trayManager->frameRenderingQueued(evt);

    // update information widgets
    auto infoPanel = dynamic_cast<OgreBites::ParamsPanel*>(trayManager->getWidget("infoPanel"));
    infoPanel->setParamValue(0, Ogre::StringConverter::toString(controller->getPolar()->getHeartRate()));
    infoPanel->setParamValue(1, Ogre::StringConverter::toString(controller->getBicycle()->getRpmSpeed()));
    infoPanel->setParamValue(2, Ogre::StringConverter::toString(controller->getBicycle()->getFriction()));

    auto ammoPanel = dynamic_cast<OgreBites::ParamsPanel*>(trayManager->getWidget("ammoPanel"));
    ammoPanel->setParamValue(0, Ogre::StringConverter::toString(controller->getLogicManager()->getMonsterHealth()));
    ammoPanel->setParamValue(1, Ogre::StringConverter::toString(controller->getLogicManager()->getPlayerAmmo()));
    ammoPanel->setParamValue(2, Ogre::StringConverter::toString(controller->getTerrainManager()->getBulletCount()));
    ammoPanel->setParamValue(3, controller->getReplenishAmmoRemainingClockAsSeconds());

    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("clockLabel"))->setCaption("Session Time: " + controller->getElapsedTimeAsString());

    // widgets update on debug mode only
    if(controller->getDebug()) {
        Ogre::Vector3 realCoord = controller->getLogicManager()->getPlayerNode()->getPosition();
        std::pair<int,int> textCoord = controller->getTerrainManager()->getCollisionCoordinates(realCoord);
        auto debugPanel = dynamic_cast<OgreBites::ParamsPanel*>(trayManager->getWidget("debugPanel"));
        debugPanel->setParamValue(0, Ogre::StringConverter::toString(controller->getTerrainManager()->getTerrainAt(realCoord).first));
        debugPanel->setParamValue(1, Ogre::StringConverter::toString(realCoord));
        debugPanel->setParamValue(2, Ogre::StringConverter::toString(textCoord.first));
        debugPanel->setParamValue(3, Ogre::StringConverter::toString(textCoord.second));
    }
}

void HUD::setupRunnerMode() {
    // update context mode
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("contextLabel"))->setCaption("Runner Mode");
    trayManager->destroyWidget("contextLogo");
    trayManager->createDecorWidget(CONTEXT_TL, "contextLogo", "Cycleshooter/RunningModePanel");
}

void HUD::setupShooterMode() {
    // update context mode
    dynamic_cast<OgreBites::Label*>(trayManager->getWidget("contextLabel"))->setCaption("Shooter Mode");
    trayManager->destroyWidget("contextLogo");
    trayManager->createDecorWidget(CONTEXT_TL, "contextLogo", "Cycleshooter/ShooterModePanel");
}

void HUD::setDebug(bool debug) {
    // fps
    debug ? trayManager->showFrameStats(FPS_TL) : trayManager->hideFrameStats();

    if(debug) {
        if(!trayManager->getWidget("debugPanel")) {
            Ogre::StringVector params = {"Terrain Type","Position","Transform x","Transform y"};
            Ogre::StringVector values = Ogre::StringVector(params.size(), "");
            trayManager->createParamsPanel(DEBUG_PANEL_TL, "debugPanel", 300, params)->setAllParamValues(values);
        }
        else {
            trayManager->moveWidgetToTray("debugPanel", DEBUG_PANEL_TL, 0);
            trayManager->getWidget("debugPanel")->show();
        }
    }
    else {
        if(trayManager->getWidget("debugPanel")) {
            trayManager->removeWidgetFromTray("debugPanel");
            trayManager->getWidget("debugPanel")->hide();
        }
    }
}

}
