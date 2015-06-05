#include "LogicManager.hpp"

namespace Cycleshooter {

int LogicManager::getPlayerHeartRate() const {
    return playerHeartRate;
}

void LogicManager::setPlayerHeartRate(const int &value) {
    playerHeartRate = value;
}

int LogicManager::getPlayerAmmo() const {
    return playerAmmo;
}

LogicManager::LogicManager(Controller* controller) :
    controller(controller)
{
    go();
}

void LogicManager::update(const Ogre::FrameEvent &evt) {
    // TODO: populate this method
}

void LogicManager::shoot() {
    std::cout << "--> LogicManager: shoot <--" << std::endl;

    if(playerAmmo > 0) {
        decrementPlayerAmmo();
        AudioManager::instance().random_play_shoot();

        // TODO: (maybe) replenish ammo in the map / terrain / collision part?
        // TODO: add several sound effects for each outcome
        // TODO: RTT crosshair + monster logic
        // TODO: if hit, then decrease monster life
        // TODO: check if monster is dead
    }
    else {
        std::cout << " |-> No more ammo" << std::endl;
    }
}

Ogre::SceneNode *LogicManager::getPlayerSceneNode() const {
    return parentPlayerSceneNode;
}

int LogicManager::getMonsterHealth() const {
    return monsterHealth;
}

void LogicManager::incrementPlayerAmmo(int quantity) {
    Ogre::LogManager::getSingleton().logMessage("--> LogicManager: Increment Player Ammo <--");

    playerAmmo += quantity;
}

void LogicManager::decrementPlayerAmmo(int quantity) {
    Ogre::LogManager::getSingleton().logMessage("--> LogicManager: Decrement Player Ammo <--");

    playerAmmo = std::max(0, playerAmmo - quantity);
}

void LogicManager::go() {
    Ogre::LogManager::getSingleton().logMessage("--> LogicManager: go <--");

    createCameras();
    createSceneNodes();
    createViewports();
}

void LogicManager::createCameras() {
    Ogre::LogManager::getSingleton().logMessage("--> LogicManager: Creating Cameras <--");

    // create cameras
    frontCamera = controller->getSceneManager()->createCamera("frontCamera");
    rearCamera = controller->getSceneManager()->createCamera("rearCamera");

    // adjust clip distances in cameras
    frontCamera->setNearClipDistance(CAMERA_NEAR_CLIP_DISTANCE);
    frontCamera->setFarClipDistance(CAMERA_FAR_CLIP_DISTANCE);
    rearCamera->setNearClipDistance(CAMERA_NEAR_CLIP_DISTANCE);
    frontCamera->setFarClipDistance(CAMERA_FAR_CLIP_DISTANCE);
}

void LogicManager::createSceneNodes() {
    Ogre::LogManager::getSingleton().logMessage("--> LogicManager: Creating Scene Nodes <--");

    // create scene nodes
    parentPlayerSceneNode = controller->getSceneManager()->getRootSceneNode()->createChildSceneNode("parentPlayerSceneNode");
    frontPlayerSceneNode = parentPlayerSceneNode->createChildSceneNode("frontPlayerSceneNode");
    rearPlayerSceneNode = parentPlayerSceneNode->createChildSceneNode("rearPlayerSceneNode");
    rearPlayerSceneNode->yaw(Ogre::Radian(Ogre::Degree(180.0)));

    // attach scene nodes
    frontPlayerSceneNode->attachObject(frontCamera);
    rearPlayerSceneNode->attachObject(rearCamera);
}

void LogicManager::createViewports() {
    Ogre::LogManager::getSingleton().logMessage("--> LogicManager: Creating Viewports <--");

    viewportFull = controller->getWindow()->addViewport(frontCamera, 0);
}

void LogicManager::setupRunnerMode() {
    viewportFull->setCamera(frontCamera);
    frontCamera->setAspectRatio(Ogre::Real(viewportFull->getActualWidth()) / Ogre::Real(viewportFull->getActualHeight()));

    viewportMirror = controller->getWindow()->addViewport(rearCamera, 1, (1.0 - MIRROR_PERCENTAGE_H)/2.0, 0.0, MIRROR_PERCENTAGE_H, MIRROR_PERCENTAGE_V);
    viewportMirror->setOverlaysEnabled(false);
    viewportMirror->setClearEveryFrame(true, Ogre::FBT_DEPTH);  // alternatively, use setClearEveryFrame(false);
    rearCamera->setAspectRatio(Ogre::Real(viewportMirror->getActualWidth()) / Ogre::Real(viewportMirror->getActualHeight()));
}

void LogicManager::setupShooterMode() {
    viewportFull->setCamera(rearCamera);
    rearCamera->setAspectRatio(Ogre::Real(viewportFull->getActualWidth()) / Ogre::Real(viewportFull->getActualHeight()));

    controller->getWindow()->removeViewport(1);
}

void LogicManager::setDebugOn() {
    controller->getSceneManager()->setDisplaySceneNodes(true);
    controller->getSceneManager()->showBoundingBoxes(true);

    // bonus
    playerAmmo += 10;
}

void LogicManager::setDebugOff() {
    controller->getSceneManager()->setDisplaySceneNodes(false);
    controller->getSceneManager()->showBoundingBoxes(false);
}

}
