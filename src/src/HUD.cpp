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

    // widgets update on debug mode only
    if(controller->getDebug()) {
        Ogre::Vector3 realCoord = controller->getLogicManager()->getPlayerNode()->getPosition();
        std::pair<int,int> textCoord = controller->getTerrainManager()->getCollisionCoordinates(realCoord);
        //std::cout <<Ogre::StringConverter::toString(realCoord) <<std::endl;
        //std::cout <<realCoord <<std::endl;
        dynamic_cast<OgreBites::ParamsPanel*>(trayManager->getWidget("debugPanel"))->setParamValue(0, Ogre::StringConverter::toString(controller->getTerrainManager()->getTerrainAt(realCoord)));
        dynamic_cast<OgreBites::ParamsPanel*>(trayManager->getWidget("debugPanel"))->setParamValue(1, Ogre::StringConverter::toString(realCoord));
        dynamic_cast<OgreBites::ParamsPanel*>(trayManager->getWidget("debugPanel"))->setParamValue(2, Ogre::StringConverter::toString(textCoord.first));
        dynamic_cast<OgreBites::ParamsPanel*>(trayManager->getWidget("debugPanel"))->setParamValue(3, Ogre::StringConverter::toString(textCoord.second));
    }
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

    // debug panel
    if(!trayManager->getWidget("debugPanel")) {
        Ogre::StringVector params = {"Terrain Type","Position","Transform x","Transform y"};
        Ogre::StringVector values = {"","","",""};
        trayManager->createParamsPanel(DEBUG_PANEL_TL, "debugPanel", 300, params)->setAllParamValues(values);
    }
    else {
        trayManager->moveWidgetToTray("debugPanel", DEBUG_PANEL_TL, 0);
        trayManager->getWidget("debugPanel")->show();
    }
}

void HUD::setupDebugOff() {
    // fps
    trayManager->hideFrameStats();

    // debug panel
    if(trayManager->getWidget("debugPanel")) {
        trayManager->removeWidgetFromTray("debugPanel");
        trayManager->getWidget("debugPanel")->hide();
    }
}

}
