#include "LogicManager.hpp"

namespace Cycleshooter {

int LogicManager::getPlayerAmmo() const {
    return playerAmmo;
}

LogicManager::LogicManager(Controller* controller) :
    controller(controller)
{
    int numOfTerrainTypes = 7;
    difficultyParamenter = std::vector<float> (numOfTerrainTypes,0);
    setDifficultyParamenter();
    go();
}

void LogicManager::update(const Ogre::FrameEvent &evt) {
    auto elapsedTime = evt.timeSinceLastFrame;

    if(controller->getContext() == CONTEXT_RUNNER) {
        updatePlayerPosition(elapsedTime);
    }

    updateMonsterPosition(elapsedTime);

    if(checkPlayerMonsterCollision()) {
        // TODO: maybe pass an enum to this function so we know exactly why the game ended. Define it on the Simple/ directory.
        controller->shutdownNow(false);
    }
}

void LogicManager::shoot() {
    std::cout << "--> LogicManager: shoot <--" << std::endl;

    if(decrementPlayerAmmo()) {
        AudioManager::instance().playRandomShoot();

        Ogre::SceneNode* monsterNode = controller->getSceneManager()->getSceneNode("monsterNode");

        monsterNode->flipVisibility();
        controller->getSceneManager()->getRootSceneNode()->flipVisibility();
        bool debug = controller->getDebug(), setDebugOff();

        rttRenderTarget->update();

        Ogre::Image rttImage;
        rttTexture->convertToImage(rttImage);

        std::pair<int, int> coords = controller->getCrosshairManager()->convertToImageCoordinates(rttImage);

        if (rttImage.getColourAt(coords.first, coords.second, 0) != Ogre::ColourValue::Black) {
            AudioManager::instance().playSound(SOUND_MONSTER_HIT);
            decrementMonsterHealth();
        }

        if(debug) setDebugOn();
        controller->getSceneManager()->getRootSceneNode()->flipVisibility();
        monsterNode->flipVisibility();
    }
    else {
        // no more ammo
        AudioManager::instance().playSound(SOUND_DRY_SHOOT);
    }
}

Ogre::SceneNode *LogicManager::getPlayerNode() const {
    return parentPlayerNode;
}

int LogicManager::getMonsterHealth() const {
    return monsterHealth;
}

void LogicManager::decrementMonsterHealth(int quantity) {
    Ogre::LogManager::getSingleton().logMessage("--> LogicManager: Decrement Monster Health <--");

    monsterHealth = std::max(0, monsterHealth - quantity);

    if(monsterHealth <= 0) {
        controller->shutdownNow(true);
    }
}

void LogicManager::updatePlayerPosition(const Ogre::Real &time) {
    // distance = speed x time (Physics I, yay!)
    double distance = controller->getBicycle()->getGameSpeed() * time;

    Ogre::Vector3 playerOrientation = frontCamera->getDirection();

    getPlayerNode()->translate(distance * playerOrientation, Ogre::SceneNode::TS_LOCAL);
}

void LogicManager::updateMonsterPosition(const Ogre::Real &time) {
    auto monsterSpeed = 200.0;
    Ogre::SceneNode* monsterNode = controller->getSceneManager()->getSceneNode("monsterNode");

    // distance = speed x time (Physics I, yay!)
    double distance = monsterSpeed * time;

    // upstream: http://stackoverflow.com/questions/4727079/getting-object-direction-in-ogre
    Ogre::Vector3 monsterOrientation = monsterNode->getOrientation() * Ogre::Vector3::UNIT_Z;

    monsterNode->translate(distance * monsterOrientation, Ogre::SceneNode::TS_LOCAL);
}

bool LogicManager::checkPlayerMonsterCollision() {
    Ogre::SceneNode* monsterNode = controller->getSceneManager()->getSceneNode("monsterNode");

    Ogre::Real thresholdDistance = controller->getSceneManager()->getEntity("monsterEntity")->getBoundingRadius();

    return monsterNode->getPosition().squaredDistance(parentPlayerNode->getPosition()) < thresholdDistance * thresholdDistance;
}

void LogicManager::incrementPlayerAmmo(int quantity) {
    Ogre::LogManager::getSingleton().logMessage("--> LogicManager: Increment Player Ammo <--");

    playerAmmo += quantity;
}

bool LogicManager::decrementPlayerAmmo(int quantity) {
    Ogre::LogManager::getSingleton().logMessage("--> LogicManager: Decrement Player Ammo <--");

    if(playerAmmo - quantity >= 0) {
        playerAmmo -= quantity;
        return true;
    }
    else {
        return false;
    }
}

void LogicManager::go() {
    Ogre::LogManager::getSingleton().logMessage("--> LogicManager: go <--");

    createCameras();
    createSceneNodes();
    createViewports();
    createRtt();
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
    parentPlayerNode = controller->getSceneManager()->getRootSceneNode()->createChildSceneNode("parentPlayerNode");
    frontPlayerNode = parentPlayerNode->createChildSceneNode("frontPlayerNode");
    rearPlayerNode = parentPlayerNode->createChildSceneNode("rearPlayerNode");
    rearPlayerNode->yaw(Ogre::Radian(Ogre::Degree(180.0)));

    // attach scene nodes
    frontPlayerNode->attachObject(frontCamera);
    rearPlayerNode->attachObject(rearCamera);
}

void LogicManager::createViewports() {
    Ogre::LogManager::getSingleton().logMessage("--> LogicManager: Creating Viewports <--");

    viewportFull = controller->getWindow()->addViewport(frontCamera, 0);
}

void LogicManager::createRtt() {
    rttTexture = Ogre::TextureManager::getSingleton().createManual(
                "rttTexture",
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                Ogre::TEX_TYPE_2D,
                controller->getWindow()->getWidth(), controller->getWindow()->getHeight(),
                0,
                Ogre::PF_R8G8B8,
                Ogre::TU_RENDERTARGET);

    rttRenderTarget = rttTexture->getBuffer()->getRenderTarget();
    rttRenderTarget->addViewport(rearCamera);
    rttRenderTarget->setAutoUpdated(false);
    rttRenderTarget->getViewport(0)->setClearEveryFrame(true);
    rttRenderTarget->getViewport(0)->setOverlaysEnabled(false);
    rttRenderTarget->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
}

void LogicManager::setDifficultyParamenter() {
    difficultyParamenter[0] = 1.0f;
    difficultyParamenter[1] = 1.0f;
    difficultyParamenter[2] = 1.0f;
    difficultyParamenter[3] = 1.0f;
    difficultyParamenter[4] = 2.0f;
    difficultyParamenter[5] = 3.0f;
    difficultyParamenter[6] = 4.0f;
}

void LogicManager::externalIncrement(){
    incrementPlayerAmmo(1);
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
}

void LogicManager::setDebugOff() {
    controller->getSceneManager()->setDisplaySceneNodes(false);
    controller->getSceneManager()->showBoundingBoxes(false);
}

void LogicManager::translateMonster(int difficulty, Ogre::Vector3 translation){
    float parameter = 1/difficultyParamenter[difficulty];
    parentPlayerNode->translate(translation*parameter);
}

}
