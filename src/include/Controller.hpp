#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include <SFML/System.hpp>
#include <Ogre.h>
#include <OgreOverlaySystem.h>

#include "LogicManager.hpp"
#include "RandomPolar.hpp"

#include "Context.hpp"
#include "HUD.hpp"
#include "NodeManager.hpp"
#include "TerrainManager.hpp"
#include "CollisionHandler.hpp"

namespace Cycleshooter {

class HUD;
class NodeManager;
class TerrainManager;
class CollisionHandler;

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
    void polarUpdaterFunction();

    /**
     * @brief context the current game context mode
     */
    Context context = CONTEXT_RUNNER;

    /**
     * @brief debug indicates whether the debug mode is activated or not
     */
    bool debug = false;

    /**
     * @brief shutdown setting this to true will stop several while loops, thus forcing the game to finish cleanly
     */
    bool shutdown = false;

    /**
     * @brief hud responsible for rendering overlay elements with useful information for the player
     */
    HUD* hud = NULL;

    LogicManager* logicManager = NULL;
    NodeManager* nodeManager = NULL;
    TerrainManager* terrainManager = NULL;
    CollisionHandler* collisionHandler = NULL;

    Ogre::Root *oRoot = NULL;
    Ogre::SceneManager *oSceneManager = NULL;
    Ogre::OverlaySystem *oOverlaySystem = NULL;

    // customizable settings
    const Ogre::String RENDER_WINDOW_NAME = "Cycleshooter Render Window";
    const Ogre::String RESOURCES_CONFIG = "resources.cfg";
    const Ogre::String MAIN_TEXTURE = "racecircuit.png";
    const sf::Time POLAR_SLEEP_TIME = sf::milliseconds(500);

    // go
    void go();
    void createRoot();
    void createSceneManager();
    void createOverlaySystem();
    void setupResources();

public:
    Controller();
    virtual ~Controller();

    /**
     * The game main loop. Updates logic and rendering.
     */
    void gameMainLoop();

    /**
     * Setup the context mode to the runner mode.
     */
    void setupRunnerMode();

    /**
     * Setup the context mode to the shooter mode.
     */
    void setupShooterMode();

    /**
     * Toggle the context mode Shooter/Runner.
     */
    void toggleMode();

    /**
     * Toggle the context mode to the specified context.
     */
    void toggleMode(const Context& newContext);

    /**
     * Enable the debug mode.
     */
    void setupDebugOn();

    /**
     * Disable the debug mode.
     */
    void setupDebugOff();

    /**
     * Toggle the debug mode ON/OFF.
     */
    void toggleDebug();

    /**
     * Return true if debug is activated.
     */
    bool isDebugOn() const;

    /**
     * Finish/shutdown the game cleanly.
     */
    void shutdownNow();

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
    sf::Thread *getPolarUpdater() const;
    LogicManager *getLogicManager() const;
};

}

#endif
