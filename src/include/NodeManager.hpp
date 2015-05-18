#ifndef _NODEMANAGER_HPP_
#define _NODEMANAGER_HPP_

#include <Ogre.h>
#include <stdexcept>

#include "Controller.hpp"

namespace Cycleshooter {
class Controller;

class NodeManager {
    Controller* controller = NULL;

    Ogre::Viewport *viewportPrimary = NULL;
    Ogre::Viewport *viewportSecundary = NULL;

    Ogre::SceneNode* parentPlayerSceneNode = NULL;
    Ogre::SceneNode* frontPlayerSceneNode = NULL;
    Ogre::SceneNode* rearPlayerSceneNode = NULL;

    Ogre::Camera *frontCamera = NULL;
    Ogre::Camera *rearCamera = NULL;

    void go();
    void createCameras();
    void createSceneNodes();

    // customizable settings
    const double CAMERA_NEAR_CLIP_DISTANCE = 5.0;
    const double CAMERA_FAR_CLIP_DISTANCE = 10000.0;
    const double MIRROR_PERCENTAGE_H = 0.65;
    const double MIRROR_PERCENTAGE_V = 0.15;
    const Ogre::ColourValue VIEWPORT_BACKGROUND_COLOR = Ogre::ColourValue(0.0, 0.0, 0.0);

public:
    NodeManager(Controller* controller);
    virtual ~NodeManager();

    void setupRunnerMode();
    void setupShooterMode();
    void setupNoneMode();

    // getters and setters
    Controller *getController() const;
    Ogre::Viewport *getViewportPrimary() const;
    Ogre::Viewport *getViewportSecundary() const;
    Ogre::SceneNode *getParentPlayerSceneNode() const;
    Ogre::SceneNode *getFrontPlayerSceneNode() const;
    Ogre::SceneNode *getRearPlayerSceneNode() const;
    Ogre::Camera *getFrontCamera() const;
    Ogre::Camera *getRearCamera() const;
    Ogre::Camera *getMainCamera() const;
};
}

#endif
