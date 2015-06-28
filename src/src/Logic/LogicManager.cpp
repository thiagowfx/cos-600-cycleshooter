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
    int numOfTerrainTypes = 7;
    difficultyParamenter = std::vector<float> (numOfTerrainTypes,0);
    setDifficultyParamenter();
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

        Ogre::SceneNode* monsterNode = controller->getSceneManager()->getSceneNode("monsterNode");

        monsterNode->flipVisibility();
        controller->getSceneManager()->getRootSceneNode()->flipVisibility();
        bool debug = controller->getDebug(), setDebugOff();

        rttRenderTarget->update();

        Ogre::Image rttImage;
        rttTexture->convertToImage(rttImage);

        auto crosshair_to_img_coords = [](std::pair<double, double> crosshair, Ogre::Image& image) -> std::pair<int, int> {
            // map [-1.0, +1.0] to [0, width)
            int retx = ((crosshair.first + 1.0) * image.getWidth()) / 2.0;

            // map [-1.0, +1.0] to [0, height)
            // int rety = ((crosshair.second + 1.0) * image.getHeight()) / 2.0;
            int rety = image.getHeight() - static_cast<int>(((crosshair.second + 1.0) * image.getHeight()) / 2.0);

            if(retx == image.getWidth())
                --retx;

            if(rety == image.getHeight())
                --rety;

            return std::make_pair(retx, rety);
        };

        auto coords = crosshair_to_img_coords(controller->getCrosshairManager()->getScroll(), rttImage);

        if (rttImage.getColourAt(coords.first, coords.second, 0) != Ogre::ColourValue::Black) {
            decrementMonsterHealth();

            if(monsterHealth == 0) {
                system("echo Congratulations you won");
                controller->shutdownNow();
            }
        }

        // TODO: (maybe) replenish ammo in the map / terrain / collision part?
        // TODO: add several sound effects for each outcome

        if(debug) setDebugOn();
        controller->getSceneManager()->getRootSceneNode()->flipVisibility();
        monsterNode->flipVisibility();
    }
    else {
        std::cout << " |-> No more ammo" << std::endl;
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
void LogicManager::setDifficultyParamenter(){
    difficultyParamenter[0] = 1.0f;
    difficultyParamenter[1] = 1.0f;
    difficultyParamenter[2] = 1.0f;
    difficultyParamenter[3] = 1.0f;
    difficultyParamenter[4] = 2.0f;
    difficultyParamenter[5] = 3.0f;
    difficultyParamenter[6] = 4.0f;
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

void LogicManager::translateMonster(int difficulty, Ogre::Vector3 translation){
    float parameter = 1/difficultyParamenter[difficulty];
    parentPlayerNode->translate(translation*parameter);
}

}
