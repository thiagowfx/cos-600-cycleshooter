#ifndef _CONTEXTMANAGER_CPP_
#define _CONTEXTMANAGER_CPP_

#include <SFML/System.hpp>

#include "OgreCamera.h"
#include "OgreSceneNode.h"
#include "OgreViewport.h"

#include "OgreRenderWindow.h"
#include "OgreSceneManager.h"

namespace Cycleshooter {
class ContextManager : sf::NonCopyable {
public:
    enum ContextMode {
        CONTEXT_RUNNER,
        CONTEXT_SHOOTER,
        CONTEXT_NONE
    };

private:
    ContextMode context;

    Ogre::Viewport *viewportPrimary;
    Ogre::Viewport *viewportSecundary;

    Ogre::SceneNode* cameraParentSceneNode;
    Ogre::SceneNode* cameraFrontSceneNode;
    Ogre::SceneNode* cameraRearSceneNode;

    Ogre::Camera *cameraFront;
    Ogre::Camera *cameraRear;

    // third-party members
    Ogre::SceneManager* sceneManager;
    Ogre::RenderWindow* renderWindow;

public:
    ContextManager(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* renderWindow);
    virtual ~ContextManager();

    void createCameras();
    void setupRunnerMode();
    void setupShooterMode();
    void setupNoneMode();
    void toggleMode();
    void toggleMode(const ContextMode& mode);

    Ogre::Camera* getFrontCamera() const;
    Ogre::Camera* getRearCamera() const;
    Ogre::Camera* getMainCamera() const;
    Ogre::SceneNode* getCameraParentSceneNode() const;
    Ogre::SceneNode* getCameraFrontSceneNode() const;
    Ogre::SceneNode* getCameraRearSceneNode() const;
};
}

#endif
