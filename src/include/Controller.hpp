#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include <getopt.h>
#include <cstdlib>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <Ogre.h>
#include <OgreOverlaySystem.h>

#include "AudioManager.hpp"
#include "CycleshooterIcon.hpp"
#include "Context.hpp"
#include "InputManager.hpp"
#include "LogicManager.hpp"

#include "ConstantPolar.hpp"
#include "RandomPolar.hpp"
#include "RealPolar.hpp"

#include "CrosshairManager.hpp"
#include "HUD.hpp"
#include "CollisionHandler.hpp"
#include "TerrainManager.hpp"

namespace Cycleshooter {

class HUD;
class LogicManager;
class CollisionHandler;
class TerrainManager;

class Controller : public sf::NonCopyable, public Ogre::FrameListener {
    /**
     * @brief context the current game context mode
     */
    Context context;

    /**
     * @brief debug indicates whether the debug mode is activated or not
     */
    bool debug;

    /**
     * @brief shutdown setting this to true will stop several while loops, thus forcing the game to finish cleanly
     */
    bool shutdown = false;

    /**
     * @brief gameWon Determines whether the player won or lost the game should it end.
     */
    bool gameWon = false;

    /**
     * Wait for other threads to finish. Like 'join' for C++11 threads.
     */
    void wait_threads() const;

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
    std::unique_ptr<sf::Window> sWindow;

    /**
     * @brief sFullScreen State of the game window: will it be fullscreen?
     */
    int sFullScreen;

    /**
     * @brief sVideoMode Resolution of the game window (e.g. 800 x 600)
     */
    sf::VideoMode sVideoMode;

    /**
     * @brief APPLICATION_NAME Name of our application
     */
    const Ogre::String APPLICATION_NAME = "Cycleshooter";

    /**
     * @brief POLAR_SLEEP_TIME Time between polar updates.
     */
    const sf::Time POLAR_SLEEP_TIME = sf::milliseconds(500);

    /**
     * @brief POLAR_PEAK_CHANGE Variation of the heart beat between sucessive increments/decrements.
     */
    const int POLAR_PEAK_CHANGE = 5;

    /**
     * @brief HEARTBEAT_PLAY_CHECK_PERIOD Time period to check if a heart beat sound should be played or not.
     */
    const sf::Time HEARTBEAT_PLAY_CHECK_PERIOD = sf::milliseconds(220);

    /**
     * @brief HEARTBEAT_MINIMUM_ASSUMED The minimumum expected heart beat. Used to choose a sound.
     */
    const int HEARTBEAT_MINIMUM_ASSUMED = 75;

    /**
     * @brief HEARTBEAT_MAXIMUM_ASSUMED The maximum expected heart beat. Used to choose a sound.
     */
    const int HEARTBEAT_MAXIMUM_ASSUMED = 150;

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
     * Create the crosshair as an overlay element.
     */
    void createCrosshair();

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
    std::unique_ptr<LogicManager> logicManager;

    /**
     * The polar device, from where we will get the heart rates.
     */
    std::unique_ptr<AbstractPolar> polar;

    /**
     * The thread responsible for updating the heart rate.
     */
    std::unique_ptr<sf::Thread> polarUpdater;

    /**
     * Terrain Manager.
     */
    std::unique_ptr<TerrainManager> terrainManager;

    /**
     * Collision Handler.
     */
    std::unique_ptr<CollisionHandler> collisionHandler;

    /**
     * Crosshair Manager.
     */
    std::unique_ptr<CrosshairManager> crosshairManager;

    /**
     * @brief hud responsible for rendering overlay elements with useful information for the player
     */
    std::unique_ptr<HUD> hud;

    /**
     * @brief frameRenderingQueued Overriden from Ogre::FrameListener.
     * This should be used for performancing purposes (efficiently use CPU time while GPU is working elsewhere).
     */
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    /**
     * The (manually managed) game main loop, responsible for calling Ogre::Root::renderOneFrame()
     */
    void gameMainLoop();

    /**
     * @brief do_game_end The game has ended. What should we do?
     */
    void do_game_end();

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

public:
    Controller(int argc, char *argv[]);

    /**
     * Finish/shutdown the game cleanly.
     */
    void shutdownNow(bool gameWon);

    // getters and setters
    LogicManager* getLogicManager() const;
    Context getContext() const;
    Ogre::RenderWindow *getWindow() const;
    Ogre::SceneManager *getSceneManager() const;
    bool getShutdown() const;
    bool getDebug() const;
    TerrainManager* getTerrainManager() const;
    CrosshairManager* getCrosshairManager() const;
    AbstractPolar* getPolar() const;
};

}

#endif
