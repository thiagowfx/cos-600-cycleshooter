#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include <SFML/System.hpp>
#include <Ogre.h>
#include <OgreOverlaySystem.h>

#include "ConstantPolar.hpp"
#include "RandomPolar.hpp"

#include "NodeManager.hpp"
#include "HUD.hpp"
#include "TerrainManager.hpp"
#include "CollisionHandler.hpp"

namespace Cycleshooter {

class NodeManager;
class HUD;
class TerrainManager;
class CollisionHandler;

enum Context {
    CONTEXT_RUNNER,
    CONTEXT_SHOOTER,
};

enum Debug {
    DEBUG_ON,
    DEBUG_OFF,
};

class Controller : sf::NonCopyable {

    Context context;
    Debug debug = DEBUG_OFF;

    NodeManager* nodeManager = NULL;
    HUD* hud = NULL;
    TerrainManager* terrainManager = NULL;
    CollisionHandler* collisionHandler = NULL;
    AbstractPolar* polar = NULL;

    Ogre::Root *oRoot = NULL;
    Ogre::SceneManager *oSceneManager = NULL;
    Ogre::OverlaySystem *oOverlaySystem = NULL;

    // customizable settings
    const Ogre::String RENDER_WINDOW_NAME = "CYCLESHOOTER Render Window";
    const Ogre::String RESOURCES_CONFIG = "resources.cfg";
    const Ogre::String MAIN_TEXTURE = "racecircuit.png";

    // go
    void go();
    void setupResources();
    void createRoot();
    void createSceneManager();
    void createOverlaySystem();

public:
    Controller();
    virtual ~Controller();

    // setups
    void setupRunnerMode();
    void setupShooterMode();
    void toggleMode();
    void toggleMode(const Context& newContext);

    // debug
    void setupDebugOn();
    void setupDebugOff();
    void toggleDebug();
    void toggleDebug(const Debug& newDebug);

    // getters and setters
    Context getContext() const;
    NodeManager *getNodeManager() const;
    HUD *getHud() const;
    void setHud(HUD *value);
    Ogre::Root *getRoot() const;
    Ogre::RenderWindow *getWindow() const;
    Ogre::SceneManager *getSceneManager() const;
    Ogre::OverlaySystem *getOverlaySystem() const;
    Debug getDebug() const;
    AbstractPolar *getPolar() const;
};

}

#endif
