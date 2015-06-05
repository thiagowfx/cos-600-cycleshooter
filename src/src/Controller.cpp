#include "Controller.hpp"

namespace Cycleshooter {

Context Controller::getContext() const {
    return context;
}

Ogre::Root *Controller::getRoot() const {
    return oRoot;
}

Ogre::RenderWindow *Controller::getWindow() const {
    return oWindow;
}

Ogre::SceneManager *Controller::getSceneManager() const {
    return oSceneManager;
}

Controller::Controller(int argc, char *argv[]) {
    int fullscreen = true;
    int width = -1, height = -1;

    auto usage = [&]() {
        std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
        std::cout << "Options\n"
                     "    -h:    show this help\n"
                     "    -f:    set fullscreen (1 = ON, 0 = OFF, default = ON)\n"
                     "    -r:    set resolution (e.g. 1366x768, default = maximum possible)\n";
        std::cout << "Examples: " << argv[0] << " -r 1366x768" << std::endl;
        std::cout << "          " << argv[0] << " -f 0 -r 800x600" << std::endl;
    };

    int opt;
    while((opt = getopt(argc, argv, "f:r:h")) != EOF) {

        switch(opt) {
        case 'f':
            if(!(!strcmp(optarg, "0") || !strcmp(optarg, "1"))) {
                std::cout << "error: unrecognized fullscreen parameter" << std::endl;
                usage();
                exit(1);
            }
            fullscreen = atoi(optarg);
            break;

        case 'r':
            if (sscanf(optarg, "%dx%d", &width, &height) != 2) {
                std::cout << "error: unrecognized resolution format" << std::endl;
                usage();
                exit(1);
            }
            break;

        case 'h':
        case '?':
        default:
            usage();
            exit(0);
            break;
        }
    }

    /* Set fullscreen if it is appropriate. */
    sFullScreen = fullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar | sf::Style::Close;

    /* Set resolution if it is appropriate. */
    sVideoMode = sf::VideoMode::getFullscreenModes()[0];
    if(width != -1 && height != -1) {
        if(!fullscreen || (fullscreen && sf::VideoMode(width, height).isValid())) {
            sVideoMode = sf::VideoMode(width, height);
        }
        else {
            std::cout << "error: invalid fullscreen resolution specified. Please either set a valid resolution or disable fullscreen." << std::endl;
            exit(1);
        }
    }

    go();
}

LogicManager *Controller::getLogicManager() const {
    return logicManager.get();
}

void Controller::polarUpdaterFunction() {
    while(!shutdown) {
        try {
            unsigned heartRate = polar->getInstantaneousHeartRate();
            if(heartRate != 0) {
                logicManager->setPlayerHeartRate(heartRate);
            }
            else {
                Ogre::LogManager::getSingleton().logMessage("polarUpdaterFunction: heartRate is zero!! You're either dead or far from the control board", Ogre::LML_CRITICAL);
            }
        }
        catch (...) {
            Ogre::LogManager::getSingleton().logMessage("polarUpdaterFunction: caught exception", Ogre::LML_CRITICAL);
        }

        sf::sleep(POLAR_SLEEP_TIME);
    }
}

bool Controller::frameRenderingQueued(const Ogre::FrameEvent &evt) {
    // update windows, if necessary
    Ogre::WindowEventUtilities::messagePump();

    if(oWindow->isClosed())
        shutdownNow();

    if(shutdown) {
        // sync with other threads for a clean shutdown
        polarUpdater->wait();

        return false;
    }

    // update game logic
    logicManager->update(evt);

    // update game HUD
    hud->update(evt);

    // process unbuffered keys
    if(clockUnbuf.getElapsedTime() >= THRESHOLD_UNBUF_KEYS) {
        InputManager::instance().executeActionsUnbuf(context);
        clockUnbuf.restart();
    }

    // process events (in particular, buffered keys)
    sf::Event event;
    while (sWindow->pollEvent(event)) {
        switch (event.type) {

        // window closed
        case sf::Event::Closed:
            shutdownNow();
            sWindow->close();
            break;

        // key pressed
        case sf::Event::KeyPressed:
            InputManager::instance().executeKeyAction(event.key.code, context);
            break;

        // joystick button pressed
        case sf::Event::JoystickButtonPressed:
            InputManager::instance().executeJoystickButtonAction(event.joystickButton.button, context);
            break;

        // TODO: add mouse events
        }
    }

    return true;
}

bool Controller::getShutdown() const {
    return shutdown;
}

void Controller::shutdownNow() {
    shutdown = true;
}

void Controller::go() {
    // we can't use Ogre::LogManager before creating the Ogre::Root object
    std::cout << "--> Controller: go <--" << std::endl;

    // randomness
    srand(time(NULL));

    // initialize core OGRE elements
    createSFMLWindow();
    createRoot();
    createSceneManager();
    createOverlaySystem();
    setupResources();
    setupTextures();

    // initialize our objects and our game overall
    createGameElements();
    createScene();
    createCrosshair();
    createHud();

    // setups
    InputManager::instance().detectJoystick();
    setupRunnerMode();
    setupDebugOn();
    setupMappings();

    // Ogre::FrameListener <-- let's begin calling frameRenderingQueued
    oRoot->addFrameListener(this);
    gameMainLoop();
}

void Controller::createSFMLWindow() {
    std::cout << "--> Controller: Creating the SFML Window <--" << std::endl;

    sWindow = std::unique_ptr<sf::Window>(new sf::Window(sVideoMode, APPLICATION_NAME, sFullScreen, sf::ContextSettings(32, 8, 16)));
    sWindow->setIcon(CYCLESHOOTER_ICON.width, CYCLESHOOTER_ICON.height, CYCLESHOOTER_ICON.pixel_data);
    sWindow->setKeyRepeatEnabled(false);
    sWindow->setMouseCursorVisible(false);
}

void Controller::setupResources(const Ogre::String& config) {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Setting up Resources <--");

    Ogre::ConfigFile cf;
    cf.load(config);

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;

    while (seci.hasMoreElements()) {
        secName = seci.peekNextKey();

        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;

        for (i = settings->begin(); i != settings->end(); ++i) {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void Controller::setupTextures() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Setting up Textures <--");

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
}

void Controller::createGameElements() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Create Game Elements <--");

    // attention: logic manager should be created before any threads that will update it
    logicManager = std::unique_ptr<LogicManager>(new LogicManager(this));

    polar = std::unique_ptr<AbstractPolar>(new RandomPolar());
    polarUpdater = std::unique_ptr<sf::Thread>(new sf::Thread(&Controller::polarUpdaterFunction, this));
    polarUpdater->launch();

    // to use a material, the resource group must be initialized
    terrainManager = std::unique_ptr<TerrainManager>(new TerrainManager(oSceneManager,"racecircuit.png"));
    terrainManager->createTerrain();

    // starting collision handler after terrain initialization
    collisionHandler = std::unique_ptr<CollisionHandler>(new CollisionHandler("racecircuit.png"));
    collisionHandler->loadTensor();
}

void Controller::createScene() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Creating Scene <--");

    Ogre::Entity* ogreEntity1 = getSceneManager()->createEntity("ogrehead.mesh");
    Ogre::Entity* ogreEntity2 = getSceneManager()->createEntity("ogrehead.mesh");
    Ogre::Entity* ogreEntity3 = getSceneManager()->createEntity("ogrehead.mesh");

    Ogre::SceneNode* ogreNode1 = getSceneManager()->getRootSceneNode()->createChildSceneNode();
    Ogre::SceneNode* ogreNode2 = getSceneManager()->getRootSceneNode()->createChildSceneNode();
    Ogre::SceneNode* ogreNode3 = getSceneManager()->getRootSceneNode()->createChildSceneNode();

    ogreNode2->translate(0.0, 0.0, 400.0);
    ogreNode3->translate(0.0, 0.0, -400.0);

    ogreNode1->attachObject(ogreEntity1);
    ogreNode2->attachObject(ogreEntity2);
    ogreNode3->attachObject(ogreEntity3);

    getSceneManager()->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    Ogre::Light* light =  getSceneManager()->createLight("MainLight");
    light->setPosition(20.0, 80.0, 50.0);
}

void Controller::createCrosshair() {
    Ogre::LogManager::getSingletonPtr()->logMessage("--> Controller: Creating Crosshair <--");

    crosshairManager = std::unique_ptr<CrosshairManager>(new CrosshairManager());
}

void Controller::createHud() {
    Ogre::LogManager::getSingletonPtr()->logMessage("--> Controller: Creating HUD <--");

    hud = std::unique_ptr<HUD>(new HUD(this));
    hud->setHelpPanel({"1", "2"},{"ToggleMode", "ToggleDebug"});
}

void Controller::setupMappings() {
    /*
     * Runner mode mappings;
     */
    InputManager::instance().addKeysUnbuf({sf::Keyboard::W,
                                           sf::Keyboard::Up}, CONTEXT_RUNNER, [&]{
        logicManager->getPlayerSceneNode()->translate(Ogre::Vector3(0.0, 0.0, -10.0), Ogre::SceneNode::TS_LOCAL);
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::S,
                                           sf::Keyboard::Down}, CONTEXT_RUNNER, [&]{
        logicManager->getPlayerSceneNode()->translate(Ogre::Vector3(0.0, 0.0, +10.0), Ogre::SceneNode::TS_LOCAL);
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::A,
                                           sf::Keyboard::Left}, CONTEXT_RUNNER, [&]{
        logicManager->getPlayerSceneNode()->yaw(Ogre::Degree(+10.0));
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::D,
                                           sf::Keyboard::Right}, CONTEXT_RUNNER, [&]{
        logicManager->getPlayerSceneNode()->yaw(Ogre::Degree(-10.0));
    });

    /*
     * Shooter mode mappings.
     */
    InputManager::instance().addKeysUnbuf({sf::Keyboard::A,
                                           sf::Keyboard::Left}, CONTEXT_SHOOTER, [&]{
        crosshairManager->scroll(-0.04, 0.00);
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::D,
                                           sf::Keyboard::Right}, CONTEXT_SHOOTER, [&]{
        crosshairManager->scroll(0.04, 0.00);
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::W,
                                           sf::Keyboard::Up}, CONTEXT_SHOOTER, [&]{
        crosshairManager->scroll(0.00, 0.04);
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::S,
                                           sf::Keyboard::Down}, CONTEXT_SHOOTER, [&]{
        crosshairManager->scroll(0.00, -0.04);
    });

    InputManager::instance().addKey(sf::Keyboard::Space, CONTEXT_SHOOTER, [&]{
        logicManager->shoot();
    });

    /*
     * Both modes mappings.
     */
    InputManager::instance().addKey(sf::Keyboard::Num1, [&]{
        toggleMode();
    });

    InputManager::instance().addKey(sf::Keyboard::Num2, [&]{
        toggleDebug();
    });

    // refresh all textures
    InputManager::instance().addKey(sf::Keyboard::F5, [&] {
       Ogre::TextureManager::getSingleton().reloadAll();
    });

    InputManager::instance().addKey(sf::Keyboard::M, [&] {
       AudioManager::instance().toggleMute();
    });

    // take a screenshot
    InputManager::instance().addKey(sf::Keyboard::Pause, [&] {
        getWindow()->writeContentsToTimestampedFile("screenshot", ".jpg");
    });

    // quit from the application
    InputManager::instance().addKeyUnbuf(sf::Keyboard::Escape, [&]{
        shutdownNow();
    });

    /*
     * Joystick mappings.
     */
    InputManager::instance().addJoystickAxisUnbuf(sf::Joystick::X, CONTEXT_RUNNER, [&](float f){
        logicManager->getPlayerSceneNode()->yaw(Ogre::Degree(-10 * f / 100.0));
    });

    InputManager::instance().addJoystickAxisUnbuf(sf::Joystick::Y, CONTEXT_RUNNER, [&](float f){
        logicManager->getPlayerSceneNode()->translate(Ogre::Vector3(0.0, 0.0, 10.0 * f / 100.0), Ogre::SceneNode::TS_LOCAL);
    });

//    InputManager::instance().addJoystickButton(0, [&]{
//        toggleMode();
//    });
}

void Controller::gameMainLoop() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Game Main Loop <--");

    while(!shutdown) {
        // update rendering
        oRoot->renderOneFrame();
    }
}

void Controller::createRoot() {
    // we can't use Ogre::LogManager before creating the Ogre::Root object
    std::cout << "--> Controller: creating Root <--" << std::endl;

    oRoot = new Ogre::Root();

    // create rendering system, but don't initialise it / the main window
    oRoot->setRenderSystem(oRoot->getAvailableRenderers().at(0));
    oRoot->initialise(false);

    Ogre::LogManager::getSingleton().logMessage("--> Controller: Ogre::Root object has been created and initialized with the default Render System (" + oRoot->getAvailableRenderers().at(0)->getName() + ")");

    Ogre::NameValuePairList misc = {{"currentGLContext", "true"}};

    // create a render window
    // note: window title and size are not important here, so we use blank values for them
    oWindow = oRoot->createRenderWindow("", 0, 0, false, &misc);
    oWindow->setVisible(true);

    Ogre::LogManager::getSingleton().logMessage("--> Controller: Render Window has been (manually) created <--");
}

void Controller::createSceneManager() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Creating Scene Manager <--");

    oSceneManager = oRoot->createSceneManager(Ogre::ST_GENERIC, "sceneManager");
}

void Controller::createOverlaySystem() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Creating Overlay System <--");

    oOverlaySystem = new Ogre::OverlaySystem();
    oSceneManager->addRenderQueueListener(oOverlaySystem);
}

void Controller::setupRunnerMode() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Setting up Runner Mode <--");

    context = CONTEXT_RUNNER;

    logicManager->setupRunnerMode();
    crosshairManager->setupRunnerMode();
    hud->setupRunnerMode();
    AudioManager::instance().play(MUSIC_RUNNER1);
}

void Controller::setupShooterMode() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Setting up Shooter Mode <--");

    context = CONTEXT_SHOOTER;

    logicManager->setupShooterMode();
    crosshairManager->setupShooterMode();
    hud->setupShooterMode();
    AudioManager::instance().play(MUSIC_SHOOTER1);
}

void Controller::toggleMode() {
    switch(context) {
    case CONTEXT_RUNNER:
        setupShooterMode();
        break;
    case CONTEXT_SHOOTER:
        setupRunnerMode();
        break;
    }
}

void Controller::setupDebugOn() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Turning Debug Mode On <--");

    debug = true;

    logicManager->setDebugOn();
    hud->setupDebugOn();
}

void Controller::setupDebugOff() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Turning Debug Mode Off <--");

    debug = false;

    logicManager->setDebugOff();
    hud->setupDebugOff();
}

void Controller::toggleDebug() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Toggling Debug Mode <--");

    if(debug) {
        setupDebugOff();
    }
    else {
        setupDebugOn();
    }
}

}
