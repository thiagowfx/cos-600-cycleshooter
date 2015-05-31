#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include <cstdlib>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <Ogre.h>
#include <OgreOverlaySystem.h>

#include "CycleshooterIcon.hpp"
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

class Controller : public sf::NonCopyable, public Ogre::FrameListener {
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

    /**
     * @brief sWindow the sfml window
     */
    sf::Window* sWindow = NULL;

    /**
     * @brief sResolution Resolution of the game window.
     */
    int sFullScreen = sf::Style::Fullscreen;

    /**
     * @brief clockUnbuf Used to create a delay between two consecutive unbuffered inputs.
     */
    sf::Clock clockUnbuf;

    /**
     * @brief APPLICATION_NAME Name of our application
     */
    const Ogre::String APPLICATION_NAME = "Cycleshooter";

    /**
     * @brief POLAR_SLEEP_TIME Time between polar updates.
     */
    const sf::Time POLAR_SLEEP_TIME = sf::milliseconds(500);

    /**
     * @brief THRESHOLD_UNBUF_KEYS Threshold between two consecutive unbuffered inputs.
     */
    const sf::Time THRESHOLD_UNBUF_KEYS = sf::milliseconds(80);

    /**
     * @brief go Our smart constructor
     */
    void go();

    /**
     * @brief createSFMLWindow Create and initialize the SFML window.
     */
    void createSFMLWindow();

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
     * Create the initial scene.
     */
    void createScene();

    /**
     * Create the HUD and its overlay widgets
     */
    void createHud();

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

    /**
     * @brief hud responsible for rendering overlay elements with useful information for the player
     */
    HUD* oHud = NULL;

    /**
     * @brief frameRenderingQueued Overriden from Ogre::FrameListener.
     * This should be used for performancing purposes (efficiently use CPU time while GPU is working elsewhere).
     */
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    //TODO: make almost everything private(?)
public:
    Controller();
    Controller(int argc, char *argv[]);
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
    Ogre::Root *getRoot() const;
    Ogre::RenderWindow *getWindow() const;
    Ogre::SceneManager *getSceneManager() const;
    bool getShutdown() const;
    sf::Thread *getPolarUpdater() const;
    LogicManager *getLogicManager() const;
};

}

#endif
