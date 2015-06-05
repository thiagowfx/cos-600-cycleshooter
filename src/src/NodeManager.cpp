#include "NodeManager.hpp"

namespace Cycleshooter {

Ogre::SceneNode *NodeManager::getParentPlayerSceneNode() const {
    return parentPlayerSceneNode;
}

NodeManager::NodeManager(Controller *controller) :
    controller(controller)
{
    go();
}

void NodeManager::go() {
    Ogre::LogManager::getSingleton().logMessage("--> NodeManager: go <--");

    createCameras();
    createSceneNodes();
    createViewports();
}

void NodeManager::createCameras() {
    Ogre::LogManager::getSingleton().logMessage("--> NodeManager: Creating Cameras <--");

    // create cameras
    frontCamera = controller->getSceneManager()->createCamera("frontCamera");
    rearCamera = controller->getSceneManager()->createCamera("rearCamera");

    // adjust clip distances in cameras
    frontCamera->setNearClipDistance(CAMERA_NEAR_CLIP_DISTANCE);
    frontCamera->setFarClipDistance(CAMERA_FAR_CLIP_DISTANCE);
    rearCamera->setNearClipDistance(CAMERA_NEAR_CLIP_DISTANCE);
    frontCamera->setFarClipDistance(CAMERA_FAR_CLIP_DISTANCE);
}

void NodeManager::createSceneNodes() {
    Ogre::LogManager::getSingleton().logMessage("--> NodeManager: Creating Scene Nodes <--");

    // create scene nodes
    parentPlayerSceneNode = controller->getSceneManager()->getRootSceneNode()->createChildSceneNode("parentPlayerSceneNode");
    frontPlayerSceneNode = parentPlayerSceneNode->createChildSceneNode("frontPlayerSceneNode");
    rearPlayerSceneNode = parentPlayerSceneNode->createChildSceneNode("rearPlayerSceneNode");
    rearPlayerSceneNode->yaw(Ogre::Radian(Ogre::Degree(180.0)));

    // attach scene nodes
    frontPlayerSceneNode->attachObject(frontCamera);
    rearPlayerSceneNode->attachObject(rearCamera);
}

void NodeManager::createViewports() {
    Ogre::LogManager::getSingleton().logMessage("--> NodeManager: Creating Viewports <--");

    viewportFull = controller->getWindow()->addViewport(frontCamera, 0);
}

void NodeManager::setupRunnerMode() {
    viewportFull->setCamera(frontCamera);
    frontCamera->setAspectRatio(Ogre::Real(viewportFull->getActualWidth()) / Ogre::Real(viewportFull->getActualHeight()));

    viewportMirror = controller->getWindow()->addViewport(rearCamera, 1, (1.0 - MIRROR_PERCENTAGE_H)/2.0, 0.0, MIRROR_PERCENTAGE_H, MIRROR_PERCENTAGE_V);
    viewportMirror->setOverlaysEnabled(false);
    viewportMirror->setClearEveryFrame(true, Ogre::FBT_DEPTH);  // alternatively, use setClearEveryFrame(false);
    rearCamera->setAspectRatio(Ogre::Real(viewportMirror->getActualWidth()) / Ogre::Real(viewportMirror->getActualHeight()));
}

void NodeManager::setupShooterMode() {
    viewportFull->setCamera(rearCamera);
    rearCamera->setAspectRatio(Ogre::Real(viewportFull->getActualWidth()) / Ogre::Real(viewportFull->getActualHeight()));

    controller->getWindow()->removeViewport(1);
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
