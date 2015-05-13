#include "ContextManager.hpp"

namespace Cycleshooter {

ContextManager::ContextManager(Ogre::Root* root) :
    viewportPrimary(0),
    viewportSecundary(0),
    parentSceneNode(0),
    frontPlayerSceneNode(0),
    rearPlayerSceneNode(0),
    frontCamera(0),
    rearCamera(0),
    sceneManager(0),
    overlaySystem(0),
    root(root),
    renderWindow(root->getAutoCreatedWindow())
{
    createSceneManagers();
    createCameras();
    createSceneNodes();
}

ContextManager::~ContextManager() {
    if(viewportPrimary)
        delete viewportPrimary;

    if(viewportSecundary)
        delete viewportSecundary;

    if(parentSceneNode)
        delete parentSceneNode;

    if(frontPlayerSceneNode)
        delete frontPlayerSceneNode;

    if(rearPlayerSceneNode)
        delete rearPlayerSceneNode;

    if(frontCamera)
        delete frontCamera;

    if(rearCamera)
        delete rearCamera;

    if(sceneManager)
        delete sceneManager;

    if(overlaySystem)
        delete overlaySystem;
}

void ContextManager::createSceneManagers() {
    sceneManager = root->createSceneManager(Ogre::ST_GENERIC);
    sceneManager->setDisplaySceneNodes(true);

    overlaySystem = new Ogre::OverlaySystem();
    sceneManager->addRenderQueueListener(overlaySystem);
}

void ContextManager::createCameras() {
    // create cameras
    frontCamera = sceneManager->createCamera("FrontCamera");
    rearCamera = sceneManager->createCamera("RearCamera");

    // adjust clip distances in cameras
    frontCamera->setNearClipDistance(CAMERA_NEAR_CLIP_DISTANCE);
    frontCamera->setFarClipDistance(CAMERA_FAR_CLIP_DISTANCE);
    rearCamera->setNearClipDistance(CAMERA_NEAR_CLIP_DISTANCE);
    frontCamera->setFarClipDistance(CAMERA_FAR_CLIP_DISTANCE);
}

void ContextManager::createSceneNodes() {
    // create scene nodes
    parentSceneNode = sceneManager->getRootSceneNode()->createChildSceneNode("ParentSceneNode");
    frontPlayerSceneNode = parentSceneNode->createChildSceneNode("FrontSceneNode");
    rearPlayerSceneNode = parentSceneNode->createChildSceneNode("RearSceneNode");
    rearPlayerSceneNode->yaw(Ogre::Radian(Ogre::Degree(180.0)));

    // attach scene nodes
    frontPlayerSceneNode->attachObject(frontCamera);
    rearPlayerSceneNode->attachObject(rearCamera);
}

void ContextManager::setupRunnerMode() {
    // clean
    renderWindow->removeAllViewports();
    contextMode = CONTEXT_RUNNER;

    // primary viewport
    viewportPrimary = renderWindow->addViewport(frontCamera, 0);
    viewportPrimary->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));

    // secondary viewport
    viewportSecundary = renderWindow->addViewport(rearCamera, 1, (1.0 - MIRROR_PERCENTAGE_H)/2.0, 0.0, MIRROR_PERCENTAGE_H, MIRROR_PERCENTAGE_V);
    viewportSecundary->setBackgroundColour(VIEWPORT_BACKGROUND_COLOR);
    viewportSecundary->setOverlaysEnabled(false);
    viewportSecundary->setClearEveryFrame(true, Ogre::FBT_DEPTH);  // alternatively, setClearEveryFrame(false);

    // adjust aspect ratios
    frontCamera->setAspectRatio(Ogre::Real(viewportPrimary->getActualWidth()) / Ogre::Real(viewportPrimary->getActualHeight()));
    rearCamera->setAspectRatio(Ogre::Real(viewportSecundary->getActualWidth()) / Ogre::Real(viewportSecundary->getActualHeight()));
}

void ContextManager::setupShooterMode() {
    // clean
    renderWindow->removeAllViewports();
    contextMode = CONTEXT_SHOOTER;

    // primary viewport
    viewportPrimary = renderWindow->addViewport(rearCamera, 0);
    viewportPrimary->setBackgroundColour(VIEWPORT_BACKGROUND_COLOR);

    // adjust aspect ratio
    rearCamera->setAspectRatio(Ogre::Real(viewportPrimary->getActualWidth()) / Ogre::Real(viewportPrimary->getActualHeight()));
}

void ContextManager::setupNoneMode() {
    // clean
    renderWindow->removeAllViewports();
    contextMode = CONTEXT_NONE;
}

void ContextManager::toggleMode() {
    switch(contextMode) {
    case CONTEXT_RUNNER:
        setupShooterMode();
        break;
    case CONTEXT_SHOOTER:
        setupRunnerMode();
        break;
    default:
        setupNoneMode();
    }
}

void ContextManager::toggleMode(const ContextManager::ContextMode& newContext) {
    if (newContext == CONTEXT_NONE && contextMode != CONTEXT_NONE)
        setupNoneMode();
    else if (newContext == CONTEXT_RUNNER && contextMode != CONTEXT_RUNNER)
        setupRunnerMode();
    else if (newContext == CONTEXT_SHOOTER && contextMode != CONTEXT_SHOOTER)
        setupShooterMode();
}

Ogre::Camera *ContextManager::getFrontCamera() const {
    return frontCamera;
}

Ogre::Camera *ContextManager::getRearCamera() const {
    return rearCamera;
}

Ogre::Camera *ContextManager::getMainCamera() const {
    if(contextMode == CONTEXT_RUNNER)
        return frontCamera;
    else if (contextMode == CONTEXT_SHOOTER)
        return rearCamera;
    else
        throw std::logic_error("getMainCamera() called in CONTEXT_NONE");
}

Ogre::SceneNode *ContextManager::getParentSceneNode() const {
    return parentSceneNode;
}

Ogre::SceneNode *ContextManager::getFrontSceneNode() const {
    return frontPlayerSceneNode;
}

Ogre::SceneNode *ContextManager::getRearSceneNode() const {
    return rearPlayerSceneNode;
}

Ogre::SceneManager *ContextManager::getSceneManager() const {
    return sceneManager;
}

}
