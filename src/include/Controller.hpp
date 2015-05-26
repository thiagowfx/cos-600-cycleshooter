#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System.hpp>

#include <Ogre.h>
#include <OgreOverlaySystem.h>

#include "Context.hpp"
#include "InputManager.hpp"
#include "LogicManager.hpp"
#include "RandomPolar.hpp"

#include "HUD.hpp"
#include "NodeManager.hpp"
#include "CollisionHandler.hpp"
#include "TerrainManager.hpp"

namespace Cycleshooter {

class HUD;
class NodeManager;
class CollisionHandler;
class TerrainManager;

class Controller : sf::NonCopyable {
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

    /**
     * @brief oRoot Ogre::Root
     */
    Ogre::Root *oRoot = NULL;

    /**
     * @brief oSceneManager Ogre::SceneManager
     */
    Ogre::SceneManager *oSceneManager = NULL;

    /**
     * @brief oOverlaySystem Ogre::OverlaySystem
     */
    Ogre::OverlaySystem *oOverlaySystem = NULL;

    /**
     * @brief oWindow Ogre::Window
     */
    Ogre::RenderWindow *oWindow = NULL;

    // customizable settings
    const Ogre::String RENDER_WINDOW_NAME = "Cycleshooter Render Window";
    const sf::Time POLAR_SLEEP_TIME = sf::milliseconds(500);

    /**
     * @brief go Our smart constructor
     */
    void go();

    /**
     * Create and initialize Ogre::Root.
     */
    void createRoot();

    /**
     * Create a scene manager.
     */
    void createSceneManager();

    /**
     * Create the overlay system.
     */
    void createOverlaySystem();

    /**
     * Setup ogre resources and initialize all resource groups.
     */
    void setupResources(const Ogre::String& config = "resources.cfg");

    /**
     * Initialize texture settings.
     */
    void setupTextures();

    /**
     * Initialize and create our game elements.
     */
    void createGameElements();

    /**
     * Setup the games shortcuts/keybindings/keymaps (mouse, joystick and keyboard)
     */
    void setupMappings();

    /**
     * @brief logicManager Manages the logic of the game.
     */
    LogicManager* logicManager = NULL;

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
     * Node Manager.
     */
    NodeManager* nodeManager = NULL;

    /**
     * Terrain Manager.
     */
    TerrainManager* terrainManager = NULL;

    /**
     * Collision Handler.
     */
    CollisionHandler* collisionHandler = NULL;

public:
    Controller();
    virtual ~Controller();

    /**
     * The (manually managed) game main loop, responsible for calling Ogre::Root::renderOneFrame()
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
