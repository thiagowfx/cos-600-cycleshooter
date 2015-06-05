#ifndef _NODEMANAGER_HPP_
#define _NODEMANAGER_HPP_

#include <Ogre.h>

#include "Controller.hpp"

namespace Cycleshooter {
class Controller;

class NodeManager {
    Controller* const controller = NULL;

    Ogre::Viewport *viewportFull = NULL;
    Ogre::Viewport *viewportMirror = NULL;

    Ogre::SceneNode* parentPlayerSceneNode = NULL;
    Ogre::SceneNode* frontPlayerSceneNode = NULL;
    Ogre::SceneNode* rearPlayerSceneNode = NULL;

    Ogre::Camera *frontCamera = NULL;
    Ogre::Camera *rearCamera = NULL;

    // go
    void go();
    void createCameras();
    void createSceneNodes();
    void createViewports();

    // customizable settings
    const double CAMERA_NEAR_CLIP_DISTANCE = 5.0;
    const double CAMERA_FAR_CLIP_DISTANCE = 10000.0;
    const double MIRROR_PERCENTAGE_H = 0.675;
    const double MIRROR_PERCENTAGE_V = 0.135;

public:
    NodeManager(Controller* controller);

    // setups
    void setupRunnerMode();
    void setupShooterMode();

    // debug
    void setDebugOn();
    void setDebugOff();

    // getters and setters
    Ogre::SceneNode *getParentPlayerSceneNode() const;
};
}

#endif
