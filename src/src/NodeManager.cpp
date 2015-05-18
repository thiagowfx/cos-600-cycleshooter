#include "NodeManager.hpp"

namespace Cycleshooter {

Controller *NodeManager::getController() const {
    return controller;
}

Ogre::Viewport *NodeManager::getViewportPrimary() const {
    return viewportPrimary;
}

Ogre::Viewport *NodeManager::getViewportSecundary() const {
    return viewportSecundary;
}

Ogre::SceneNode *NodeManager::getParentPlayerSceneNode() const {
    return parentPlayerSceneNode;
}

Ogre::SceneNode *NodeManager::getFrontPlayerSceneNode() const {
    return frontPlayerSceneNode;
}

Ogre::SceneNode *NodeManager::getRearPlayerSceneNode() const {
    return rearPlayerSceneNode;
}

Ogre::Camera *NodeManager::getFrontCamera() const {
    return frontCamera;
}

Ogre::Camera *NodeManager::getRearCamera() const {
    return rearCamera;
}

Ogre::Camera *NodeManager::getMainCamera() const {
    if(controller->getContext() == CONTEXT_RUNNER) {
        return frontCamera;
    }
    else if (controller->getContext() == CONTEXT_SHOOTER) {
        return rearCamera;
    }
    else {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "getMainCamera() called in CONTEXT_NONE", "getMainCamera()")
    }
}

NodeManager::NodeManager(Controller *controller) :
    controller(controller)
{
    go();
}

NodeManager::~NodeManager() {
    if(viewportPrimary)
        delete viewportPrimary;

    if(viewportSecundary)
        delete viewportSecundary;

    if(frontPlayerSceneNode)
        delete frontPlayerSceneNode;

    if(rearPlayerSceneNode)
        delete rearPlayerSceneNode;

    if(parentPlayerSceneNode)
        delete parentPlayerSceneNode;

    if(frontCamera)
        delete frontCamera;

    if(rearCamera)
        delete rearCamera;
}

void NodeManager::clear() {
    if(viewportPrimary)
        viewportPrimary->clear();

    if(viewportSecundary)
        viewportSecundary->clear();

    controller->getWindow()->removeAllViewports();
}

void NodeManager::go() {
    Ogre::LogManager::getSingleton().logMessage("--> NodeManager: go <--");

    createCameras();
    createSceneNodes();
}

void NodeManager::createCameras() {
    Ogre::LogManager::getSingleton().logMessage("--> Creating Cameras <--");

    // create cameras
    frontCamera = controller->getSceneManager()->createCamera("FrontCamera");
    rearCamera = controller->getSceneManager()->createCamera("RearCamera");

    // adjust clip distances in cameras
    frontCamera->setNearClipDistance(CAMERA_NEAR_CLIP_DISTANCE);
    frontCamera->setFarClipDistance(CAMERA_FAR_CLIP_DISTANCE);
    rearCamera->setNearClipDistance(CAMERA_NEAR_CLIP_DISTANCE);
    frontCamera->setFarClipDistance(CAMERA_FAR_CLIP_DISTANCE);
}

void NodeManager::createSceneNodes() {
    Ogre::LogManager::getSingleton().logMessage("--> Creating Scene Nodes <--");

    // create scene nodes
    parentPlayerSceneNode = controller->getSceneManager()->getRootSceneNode()->createChildSceneNode("ParentPlayerSceneNode");
    frontPlayerSceneNode = parentPlayerSceneNode->createChildSceneNode("FrontPlayerSceneNode");
    rearPlayerSceneNode = parentPlayerSceneNode->createChildSceneNode("RearPlayerSceneNode");
    rearPlayerSceneNode->yaw(Ogre::Radian(Ogre::Degree(180.0)));

    // attach scene nodes
    frontPlayerSceneNode->attachObject(frontCamera);
    rearPlayerSceneNode->attachObject(rearCamera);
}

void NodeManager::setupRunnerMode() {
    clear();

    // primary viewport
    viewportPrimary = controller->getWindow()->addViewport(frontCamera, 0);
    viewportPrimary->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));

    // secondary viewport
    viewportSecundary = controller->getWindow()->addViewport(rearCamera, 1, (1.0 - MIRROR_PERCENTAGE_H)/2.0, 0.0, MIRROR_PERCENTAGE_H, MIRROR_PERCENTAGE_V);
    viewportSecundary->setBackgroundColour(VIEWPORT_BACKGROUND_COLOR);
    viewportSecundary->setOverlaysEnabled(false);
    viewportSecundary->setClearEveryFrame(true, Ogre::FBT_DEPTH);  // alternatively, use setClearEveryFrame(false);

    // adjust aspect ratios
    frontCamera->setAspectRatio(Ogre::Real(viewportPrimary->getActualWidth()) / Ogre::Real(viewportPrimary->getActualHeight()));
    rearCamera->setAspectRatio(Ogre::Real(viewportSecundary->getActualWidth()) / Ogre::Real(viewportSecundary->getActualHeight()));
}

void NodeManager::setupShooterMode() {
    clear();

    // primary viewport
    viewportPrimary = controller->getWindow()->addViewport(rearCamera, 0);
    viewportPrimary->setBackgroundColour(VIEWPORT_BACKGROUND_COLOR);

    // adjust aspect ratio
    rearCamera->setAspectRatio(Ogre::Real(viewportPrimary->getActualWidth()) / Ogre::Real(viewportPrimary->getActualHeight()));
}

void NodeManager::setDebugOn() {
    controller->getSceneManager()->setDisplaySceneNodes(true);
    controller->getSceneManager()->showBoundingBoxes(true);
}

void NodeManager::setDebugOff() {
    controller->getSceneManager()->setDisplaySceneNodes(false);
    controller->getSceneManager()->showBoundingBoxes(false);
}

}
