#include "HUD.hpp"

namespace Cycleshooter {

void HUD::go() {
    Ogre::LogManager::getSingleton().logMessage("--> HUD: go <--");

    createTrayManager();
    createTrayWidgets();
}

void HUD::createTrayManager() {
    Ogre::LogManager::getSingleton().logMessage("--> HUD: Creating Tray Manager <--");

    trayManager = new OgreBites::SdkTrayManager("trayManager", controller->getWindow(), OgreBites::InputContext());
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

    // widgets update on debug mode only
    if(controller->getDebug()) {
        // TODO: matheus: add your debug info here: change controller->getShutdown() for controller->getTerrainManager->getSomeString()
        Ogre::Vector3 realCoord = controller->getNodeManager()->getParentPlayerSceneNode()->getPosition();
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

    // debug panel
    if(!trayManager->getWidget("debugPanel")) {
        const int numElements = 1;
        // TODO: fix this mess later on
        //Ogre::StringVector params = std::vector<Ogre::String>(numElements, "");
        //Ogre::StringVector values = std::vector<Ogre::String>(numElements, "");
        Ogre::StringVector params = {"Terrain Type","Position","Transform x","Transform y"};
        Ogre::StringVector values = {"","","",""};
        trayManager->createParamsPanel(DEBUG_PANEL_TL, "debugPanel", 300, params)->setAllParamValues(values);
    }
    else {
        trayManager->moveWidgetToTray("debugPanel", HELP_PANEL_TL, 0);
        trayManager->getWidget("debugPanel")->show();
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

    // debug panel
    if(trayManager->getWidget("debugPanel")) {
        trayManager->removeWidgetFromTray("debugPanel");
        trayManager->getWidget("helpPanel")->hide();
    }
}

}
