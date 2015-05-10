#ifndef _CONTEXTMANAGER_CPP_
#define _CONTEXTMANAGER_CPP_

#include <stdexcept>
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
    ContextMode contextMode;

    Ogre::Viewport *viewportPrimary;
    Ogre::Viewport *viewportSecundary;

    Ogre::SceneNode* parentSceneNode;
    Ogre::SceneNode* frontPlayerSceneNode;
    Ogre::SceneNode* rearPlayerSceneNode;

    Ogre::Camera *frontCamera;
    Ogre::Camera *rearCamera;

    // third-party members
    Ogre::SceneManager* sceneManager;
    Ogre::RenderWindow* renderWindow;

    // customizable settings
    const double CAMERA_NEAR_CLIP_DISTANCE = 5.0;
    const double CAMERA_FAR_CLIP_DISTANCE = 1000.0;
    const double MIRROR_PERCENTAGE_H = 0.75;
    const double MIRROR_PERCENTAGE_V = 0.10;
    const Ogre::ColourValue VIEWPORT_BACKGROUND_COLOR = Ogre::ColourValue(0.0, 0.0, 0.0);

public:
    ContextManager(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* renderWindow);
    virtual ~ContextManager();

    void _test();

    void createCameras();

    void setupRunnerMode();
    void setupShooterMode();
    void setupNoneMode();
    void toggleMode();
    void toggleMode(const ContextMode& mode);

    Ogre::Camera* getFrontCamera() const;
    Ogre::Camera* getRearCamera() const;
    Ogre::Camera* getMainCamera() const;
    Ogre::SceneNode* getParentSceneNode() const;
    Ogre::SceneNode* getFrontSceneNode() const;
    Ogre::SceneNode* getRearSceneNode() const;
};
}

#endif
