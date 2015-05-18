#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include <SFML/System.hpp>
#include <Ogre.h>
#include <OgreOverlaySystem.h>

#include "NodeManager.hpp"
#include "HUD.hpp"

namespace Cycleshooter {
class NodeManager;
class HUD;

enum Context {
    CONTEXT_RUNNER,
    CONTEXT_SHOOTER,
    CONTEXT_NONE
};

class Controller : sf::NonCopyable {

    Context context = CONTEXT_NONE;

    NodeManager* nodeManager = NULL;
    HUD* hud = NULL;

    Ogre::Root *oRoot = NULL;
    Ogre::SceneManager *oSceneManager = NULL;
    Ogre::OverlaySystem *oOverlaySystem = NULL;

    const Ogre::String RENDER_WINDOW_NAME = "CYCLESHOOTER Render Window";
    const Ogre::String RESOURCES_CONFIG = "resources.cfg";

    void go();
    void setupResources();
    void createRoot();
    void createSceneManager();
    void createOverlaySystem();

public:
    Controller();
    virtual ~Controller();

    void setupRunnerMode();
    void setupShooterMode();
    void setupNoneMode();
    void toggleMode();
    void toggleMode(const Context& newContext);

    // getters and setters
    Context getContext() const;
    NodeManager *getNodeManager() const;
    HUD *getHud() const;
    void setHud(HUD *value);
    Ogre::Root *getRoot() const;
    Ogre::RenderWindow *getWindow() const;
    Ogre::SceneManager *getSceneManager() const;
    Ogre::OverlaySystem *getOverlaySystem() const;
};
}

#endif
