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
    const double CAMERA_FAR_CLIP_DISTANCE = 1500.0;
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
    void setController(Controller *value);
    Ogre::Viewport *getViewportPrimary() const;
    void setViewportPrimary(Ogre::Viewport *value);
    Ogre::Viewport *getViewportSecundary() const;
    void setViewportSecundary(Ogre::Viewport *value);
    Ogre::SceneNode *getParentPlayerSceneNode() const;
    void setParentPlayerSceneNode(Ogre::SceneNode *value);
    Ogre::SceneNode *getFrontPlayerSceneNode() const;
    void setFrontPlayerSceneNode(Ogre::SceneNode *value);
    Ogre::SceneNode *getRearPlayerSceneNode() const;
    void setRearPlayerSceneNode(Ogre::SceneNode *value);
    Ogre::Camera *getFrontCamera() const;
    void setFrontCamera(Ogre::Camera *value);
    Ogre::Camera *getRearCamera() const;
    void setRearCamera(Ogre::Camera *value);

    Ogre::Camera *getMainCamera() const;
};
}

#endif
