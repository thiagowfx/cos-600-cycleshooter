#include "NodeManager.hpp"

namespace Cycleshooter {

Controller *NodeManager::getController() const {
    return controller;
}

void NodeManager::setController(Controller *value) {
    controller = value;
}

Ogre::Viewport *NodeManager::getViewportPrimary() const {
    return viewportPrimary;
}

void NodeManager::setViewportPrimary(Ogre::Viewport *value) {
    viewportPrimary = value;
}

Ogre::Viewport *NodeManager::getViewportSecundary() const {
    return viewportSecundary;
}

void NodeManager::setViewportSecundary(Ogre::Viewport *value) {
    viewportSecundary = value;
}

Ogre::SceneNode *NodeManager::getParentPlayerSceneNode() const {
    return parentPlayerSceneNode;
}

void NodeManager::setParentPlayerSceneNode(Ogre::SceneNode *value) {
    parentPlayerSceneNode = value;
}

Ogre::SceneNode *NodeManager::getFrontPlayerSceneNode() const {
    return frontPlayerSceneNode;
}

void NodeManager::setFrontPlayerSceneNode(Ogre::SceneNode *value) {
    frontPlayerSceneNode = value;
}

Ogre::SceneNode *NodeManager::getRearPlayerSceneNode() const {
    return rearPlayerSceneNode;
}

void NodeManager::setRearPlayerSceneNode(Ogre::SceneNode *value) {
    rearPlayerSceneNode = value;
}

Ogre::Camera *NodeManager::getFrontCamera() const {
    return frontCamera;
}

void NodeManager::setFrontCamera(Ogre::Camera *value) {
    frontCamera = value;
}

Ogre::Camera *NodeManager::getRearCamera() const {
    return rearCamera;
}

void NodeManager::setRearCamera(Ogre::Camera *value) {
    rearCamera = value;
}

Ogre::Camera *NodeManager::getMainCamera() const {
    if(controller->getContext() == CONTEXT_RUNNER)
        return frontCamera;
    else if (controller->getContext() == CONTEXT_SHOOTER)
        return rearCamera;
    else
        throw std::invalid_argument("getMainCamera() called in CONTEXT_NONE");
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

void NodeManager::go() {
    createCameras();
    createSceneNodes();
}

void NodeManager::createCameras() {
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
    // clean
    controller->getRoot()->getAutoCreatedWindow()->removeAllViewports();

    // primary viewport
    viewportPrimary = controller->getRoot()->getAutoCreatedWindow()->addViewport(frontCamera, 0);
    viewportPrimary->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));

    // secondary viewport
    viewportSecundary = controller->getRoot()->getAutoCreatedWindow()->addViewport(rearCamera, 1, (1.0 - MIRROR_PERCENTAGE_H)/2.0, 0.0, MIRROR_PERCENTAGE_H, MIRROR_PERCENTAGE_V);
    viewportSecundary->setBackgroundColour(VIEWPORT_BACKGROUND_COLOR);
    viewportSecundary->setOverlaysEnabled(false);
    viewportSecundary->setClearEveryFrame(true, Ogre::FBT_DEPTH);  // alternatively, use setClearEveryFrame(false);

    // adjust aspect ratios
    frontCamera->setAspectRatio(Ogre::Real(viewportPrimary->getActualWidth()) / Ogre::Real(viewportPrimary->getActualHeight()));
    rearCamera->setAspectRatio(Ogre::Real(viewportSecundary->getActualWidth()) / Ogre::Real(viewportSecundary->getActualHeight()));
}

void NodeManager::setupShooterMode() {
    // clean
    controller->getRoot()->getAutoCreatedWindow()->removeAllViewports();

    // primary viewport
    viewportPrimary = controller->getRoot()->getAutoCreatedWindow()->addViewport(rearCamera, 0);
    viewportPrimary->setBackgroundColour(VIEWPORT_BACKGROUND_COLOR);

    // adjust aspect ratio
    rearCamera->setAspectRatio(Ogre::Real(viewportPrimary->getActualWidth()) / Ogre::Real(viewportPrimary->getActualHeight()));
}

void NodeManager::setupNoneMode() {
    // clean
    controller->getRoot()->getAutoCreatedWindow()->removeAllViewports();
}

}
