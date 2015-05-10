#include "ContextManager.hpp"

namespace Cycleshooter {

ContextManager::ContextManager(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* renderWindow) :
    viewportPrimary(0),
    viewportSecundary(0),
    cameraParentSceneNode(0),
    cameraFrontSceneNode(0),
    cameraRearSceneNode(0),
    cameraFront(0),
    cameraRear(0),
    sceneManager(sceneManager),
    renderWindow(renderWindow) {
    createCameras();
}

ContextManager::~ContextManager() {
    if(viewportPrimary)
        delete viewportPrimary;

    if(viewportSecundary)
        delete viewportSecundary;

    if(cameraParentSceneNode)
        delete cameraParentSceneNode;

    if(cameraFrontSceneNode)
        delete cameraFrontSceneNode;

    if(cameraRearSceneNode)
        delete cameraRearSceneNode;

    if(cameraFront)
        delete cameraFront;

    if(cameraRear)
        delete cameraRear;
}

void ContextManager::createCameras() {
    // create cameras
    cameraFront = sceneManager->createCamera("FrontCamera");
    cameraRear = sceneManager->createCamera("RearCamera");

    // adjust clip distances in cameras
    cameraFront->setNearClipDistance(5.0);
    cameraFront->setFarClipDistance(1000.0);
    cameraRear->setNearClipDistance(5.0);
    cameraFront->setFarClipDistance(1000.0);

    // create scene nodes
    cameraParentSceneNode = sceneManager->getRootSceneNode()->createChildSceneNode("ParentSceneNode");
    cameraFrontSceneNode = cameraParentSceneNode->createChildSceneNode("FrontCameraSceneNode");
    cameraRearSceneNode = cameraParentSceneNode->createChildSceneNode("RearCameraSceneNode");
    cameraRearSceneNode->yaw(Ogre::Radian(Ogre::Degree(180.0)));

    // attach scene nodes
    cameraFrontSceneNode->attachObject(cameraFront);
    cameraRearSceneNode->attachObject(cameraRear);
}

void ContextManager::setupRunnerMode() {
    // clean
    renderWindow->removeAllViewports();
    context = CONTEXT_RUNNER;

    // primary viewport
    viewportPrimary = renderWindow->addViewport(cameraFront, 0);
    viewportPrimary->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));

    // secondary viewport
    viewportSecundary = renderWindow->addViewport(cameraRear, 1, 0.125, 0.0, 0.750, 0.10);
    viewportSecundary->setBackgroundColour(Ogre::ColourValue(0.5, 0.5, 0.5, 0.5));
    viewportSecundary->setOverlaysEnabled(false);
    viewportSecundary->setClearEveryFrame(true, Ogre::FBT_DEPTH);  // alternatively, (false);

    // adjust aspect ratios
    cameraFront->setAspectRatio(Ogre::Real(viewportPrimary->getActualWidth()) / Ogre::Real(viewportPrimary->getActualHeight()));
    cameraRear->setAspectRatio(Ogre::Real(viewportSecundary->getActualWidth()) / Ogre::Real(viewportSecundary->getActualHeight()));
}

void ContextManager::setupShooterMode() {
    // clean
    renderWindow->removeAllViewports();
    context = CONTEXT_SHOOTER;

    // primary viewport
    viewportPrimary = renderWindow->addViewport(cameraRear, 0);
    viewportPrimary->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));

    // adjust aspect ratio
    cameraRear->setAspectRatio(Ogre::Real(viewportPrimary->getActualWidth()) / Ogre::Real(viewportPrimary->getActualHeight()));
}

void ContextManager::setupNoneMode() {
    // clean
    renderWindow->removeAllViewports();
    context = CONTEXT_NONE;
}

void ContextManager::toggleMode() {
    switch(context) {
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
    if (newContext == CONTEXT_NONE && context != CONTEXT_NONE)
        setupNoneMode();
    else if (newContext == CONTEXT_RUNNER && context != CONTEXT_RUNNER)
        setupRunnerMode();
    else if (newContext == CONTEXT_SHOOTER && context != CONTEXT_SHOOTER)
        setupShooterMode();
}

Ogre::Camera *ContextManager::getFrontCamera() const {
    return cameraFront;
}

Ogre::Camera *ContextManager::getRearCamera() const {
    return cameraRear;
}

Ogre::Camera *ContextManager::getMainCamera() const {
    if(context == CONTEXT_RUNNER)
        return cameraFront;
    else if (context == CONTEXT_SHOOTER)
        return cameraRear;
    else
        throw std::exception();
}

Ogre::SceneNode *ContextManager::getCameraParentSceneNode() const {
    return cameraParentSceneNode;
}

Ogre::SceneNode *ContextManager::getCameraFrontSceneNode() const {
    return cameraFrontSceneNode;
}

Ogre::SceneNode *ContextManager::getCameraRearSceneNode() const {
    return cameraRearSceneNode;
}

}
