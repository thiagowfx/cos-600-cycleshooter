#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include <SFML/System.hpp>
#include <Ogre.h>
#include <OgreOverlaySystem.h>

#include "RandomPolar.hpp"

#include "LogicManager.hpp"
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

class Controller : sf::NonCopyable {
    /**
     * The polar device, from here we will get the heart rates.
     */
    AbstractPolar* polar = NULL;

    /**
     * The thread responsible for updating the heart rate.
     */
    sf::Thread* polarUpdater = NULL;

    /**
     * Continuously updates the heart rate in our model/logic.
     * It gets the value from the polar device.
     */
    void polarUpdaterFunction(); // TODO: add parameter (how many seconds will we sleep?)

    Context context = CONTEXT_RUNNER;
    bool debug = false;
    bool shutdown = false;

    LogicManager* logicManager = NULL;
    NodeManager* nodeManager = NULL;
    HUD* hud = NULL;
    TerrainManager* terrainManager = NULL;
    CollisionHandler* collisionHandler = NULL;

    Ogre::Root *oRoot = NULL;
    Ogre::SceneManager *oSceneManager = NULL;
    Ogre::OverlaySystem *oOverlaySystem = NULL;

    // customizable settings
    const Ogre::String RENDER_WINDOW_NAME = "CYCLESHOOTER Render Window";
    const Ogre::String RESOURCES_CONFIG = "resources.cfg";
    const Ogre::String MAIN_TEXTURE = "racecircuit.png";
    const sf::Time POLAR_SLEEP_TIME = sf::milliseconds(500);

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

    /**
     * Enable the debug mode.
     */
    void setupDebugModeOn();

    /**
     * Disable the debug mode.
     */
    void setupDebugModeOff();

    /**
     * Toggle debug mode ON/OFF.
     */
    void toggleDebugMode();

    /**
     * Return true if debug is activated.
     */
    bool isDebugModeOn() const;

    // getters and setters
    Context getContext() const;
    NodeManager *getNodeManager() const;
    HUD *getHud() const;
    void setHud(HUD *value);
    Ogre::Root *getRoot() const;
    Ogre::RenderWindow *getWindow() const;
    Ogre::SceneManager *getSceneManager() const;
    Ogre::OverlaySystem *getOverlaySystem() const;
    bool getShutdown() const;
    void shutdownNow();
    sf::Thread *getPolarUpdater() const;
    LogicManager *getLogicManager() const;
};

}

#endif
