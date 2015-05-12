#ifndef _CONTEXTMANAGER_HPP_
#define _CONTEXTMANAGER_HPP_

#include <stdexcept>
#include <SFML/System.hpp>

#include "OgreCamera.h"
#include "OgreOverlaySystem.h"
#include "OgreSceneNode.h"
#include "OgreSceneManager.h"
#include "OgreViewport.h"

#include "OgreRenderWindow.h"
#include "OgreRoot.h"

#ifdef OGRE_STATIC_LIB
#  define OGRE_STATIC_GL
#  if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#    define OGRE_STATIC_Direct3D9
// D3D10 will only work on vista, so be careful about statically linking
#    if OGRE_USE_D3D10
#      define OGRE_STATIC_Direct3D10
#    endif
#  endif
#  define OGRE_STATIC_BSPSceneManager
#  define OGRE_STATIC_ParticleFX
#  define OGRE_STATIC_CgProgramManager
#  ifdef OGRE_USE_PCZ
#    define OGRE_STATIC_PCZSceneManager
#    define OGRE_STATIC_OctreeZone
#  else
#    define OGRE_STATIC_OctreeSceneManager
#  endif
#  include "OgreStaticPluginLoader.h"
#endif

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

    Ogre::SceneManager* sceneManager;
    Ogre::OverlaySystem* overlaySystem;

    // third-party members
    Ogre::Root* root;
    Ogre::RenderWindow* renderWindow;

    // customizable settings
    const double CAMERA_NEAR_CLIP_DISTANCE = 5.0;
    const double CAMERA_FAR_CLIP_DISTANCE = 1000.0;
    const double MIRROR_PERCENTAGE_H = 0.75;
    const double MIRROR_PERCENTAGE_V = 0.10;
    const Ogre::ColourValue VIEWPORT_BACKGROUND_COLOR = Ogre::ColourValue(0.0, 0.0, 0.0);

    void createSceneManagers();
    void createCameras();
    void createSceneNodes();

public:
    ContextManager(Ogre::Root* root, Ogre::RenderWindow* renderWindow);
    virtual ~ContextManager();

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
    Ogre::SceneManager* getSceneManager() const;
};
}

#endif
