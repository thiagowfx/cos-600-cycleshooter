#include "Controller.hpp"

namespace Cycleshooter {

Context Controller::getContext() const {
    return context;
}

NodeManager *Controller::getNodeManager() const {
    return nodeManager;
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

HUD *Controller::getHud() const {
    return oHud;
}

Controller::Controller(int argc, char *argv[]) {
    std::cout << "--> Controller: Constructor <--" << std::endl;

    int fullscreen;

    if (argc >= 2) {
        fullscreen = atoi(argv[1]);

        if(!fullscreen) {
            std::cout << " |-> Disabling FullScreen Mode" << std::endl;
            sFullScreen = sf::Style::Titlebar | sf::Style::Close;
        }
    }

    if (argc >= 4) {
        int width = atoi(argv[2]);
        int height = atoi(argv[3]);

        if(!fullscreen || (fullscreen && sf::VideoMode(width, height).isValid())) {
            std::cout << " |-> Setting resolution to " + std::to_string(width) + " x " + std::to_string(height) << std::endl;
            sVideoMode = sf::VideoMode(width, height);
        }
        else {
            std::cout << " |-> Invalid fullscreen resolution specified. Please either disable fullscreen or set a valid resolution." << std::endl;
            exit(1);
        }
    }

    go();
}

Controller::~Controller() {
    if(oHud)
        delete oHud;

    if(crosshairManager)
        delete crosshairManager;

    if(collisionHandler)
        delete collisionHandler;

    if(terrainManager)
        delete terrainManager;

    if(nodeManager)
        delete nodeManager;

    if(polarUpdater)
        delete polarUpdater;

    if(polar)
        delete polar;

    if(logicManager)
        delete logicManager;

    if(sWindow)
        delete sWindow;
}

sf::Thread *Controller::getPolarUpdater() const {
    return polarUpdater;
}

void Controller::polarUpdaterFunction() {
    while(!shutdown) {
        try {
            unsigned heartRate = polar->getInstantaneousHeartRate();
            if(heartRate) {
                logicManager->setHeartRate(heartRate);
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
    oHud->update(evt);

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

LogicManager *Controller::getLogicManager() const {
    return logicManager;
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
    setupMappings();
    setupDebugOn();

    // Ogre::FrameListener <-- let's begin calling frameRenderingQueued
    oRoot->addFrameListener(this);
    gameMainLoop();
}

void Controller::createSFMLWindow() {
    std::cout << "--> Controller: Creating the SFML Window <--" << std::endl;

    sWindow = new sf::Window(sVideoMode, APPLICATION_NAME, sFullScreen, sf::ContextSettings(32, 8, 16));
    sWindow->setIcon(CYCLESHOOTER_ICON.width, CYCLESHOOTER_ICON.height, CYCLESHOOTER_ICON.pixel_data);
    sWindow->setKeyRepeatEnabled(false);
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
    logicManager = new LogicManager(this);

    polar = new RandomPolar();
    polarUpdater = new sf::Thread(&Controller::polarUpdaterFunction, this);
    polarUpdater->launch();

    nodeManager = new NodeManager(this);
    nodeManager->setupRunnerMode();

    // to use a material, the resource group must be initialized
    terrainManager = new TerrainManager(oSceneManager,"racecircuit.png");
    terrainManager->createTerrain();

    // starting collision handler after terrain initialization
    collisionHandler = new CollisionHandler("racecircuit.png");
    collisionHandler->loadTensor();
}

void Controller::createScene() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Creating Scene <--");

    Ogre::Entity* ogreEntity = getSceneManager()->createEntity("ogrehead.mesh");
    Ogre::Entity* ogreEntity2 = getSceneManager()->createEntity("ogrehead.mesh");
    Ogre::Entity* ogreEntity3 = getSceneManager()->createEntity("ogrehead.mesh");

    Ogre::SceneNode* ogreNode = getSceneManager()->getRootSceneNode()->createChildSceneNode();
    Ogre::SceneNode* ogreNode2 = getSceneManager()->getRootSceneNode()->createChildSceneNode();
    Ogre::SceneNode* ogreNode3 = getSceneManager()->getRootSceneNode()->createChildSceneNode();

    ogreNode2->translate(0.0, 0.0, 400.0);
    ogreNode3->translate(0.0, 0.0, -400.0);

    ogreNode->attachObject(ogreEntity);
    ogreNode2->attachObject(ogreEntity2);
    ogreNode3->attachObject(ogreEntity3);

    getSceneManager()->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    Ogre::Light* light =  getSceneManager()->createLight("MainLight");
    light->setPosition(20.0, 80.0, 50.0);
}

void Controller::createCrosshair() {
    Ogre::LogManager::getSingletonPtr()->logMessage("--> Controller: Creating Crosshair <--");

    crosshairManager = new CrosshairManager();
    crosshairManager->setupRunnerMode();
}

void Controller::createHud() {
    Ogre::LogManager::getSingletonPtr()->logMessage("--> Controller: Creating HUD <--");

    oHud = new HUD(this);
    oHud->setHelpPanel({"1", "2"},{"ToggleMode", "ToggleDebug"});
    oHud->setupRunnerMode();
}

void Controller::setupMappings() {
    /*
     * Runner mode mappings;
     */
    InputManager::instance().addKeysUnbuf({sf::Keyboard::W,
                                           sf::Keyboard::Up}, CONTEXT_RUNNER, [&]{
        getNodeManager()->getParentPlayerSceneNode()->translate(Ogre::Vector3(0.0, 0.0, -10.0), Ogre::SceneNode::TS_LOCAL);
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::S,
                                           sf::Keyboard::Down}, CONTEXT_RUNNER, [&]{
        getNodeManager()->getParentPlayerSceneNode()->translate(Ogre::Vector3(0.0, 0.0, +10.0), Ogre::SceneNode::TS_LOCAL);
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::A,
                                           sf::Keyboard::Left}, CONTEXT_RUNNER, [&]{
        getNodeManager()->getParentPlayerSceneNode()->yaw(Ogre::Degree(+10.0));
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::D,
                                           sf::Keyboard::Right}, CONTEXT_RUNNER, [&]{
        getNodeManager()->getParentPlayerSceneNode()->yaw(Ogre::Degree(-10.0));
    });

    /*
     * Shooter mode mappings.
     */
    InputManager::instance().addKeysUnbuf({sf::Keyboard::A,
                                           sf::Keyboard::Left}, CONTEXT_SHOOTER, [&]{
        crosshairManager->getCrosshair()->scroll(-0.04, 0.00);
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::D,
                                           sf::Keyboard::Right}, CONTEXT_SHOOTER, [&]{
        crosshairManager->getCrosshair()->scroll(0.04, 0.00);
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::W,
                                           sf::Keyboard::Up}, CONTEXT_SHOOTER, [&]{
        crosshairManager->getCrosshair()->scroll(0.00, 0.04);
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::S,
                                           sf::Keyboard::Down}, CONTEXT_SHOOTER, [&]{
        crosshairManager->getCrosshair()->scroll(0.00, -0.04);
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
        getNodeManager()->getParentPlayerSceneNode()->yaw(Ogre::Degree(-10 * f / 100.0));
    });

    InputManager::instance().addJoystickAxisUnbuf(sf::Joystick::Y, CONTEXT_RUNNER, [&](float f){
        getNodeManager()->getParentPlayerSceneNode()->translate(Ogre::Vector3(0.0, 0.0, 10.0 * f / 100.0), Ogre::SceneNode::TS_LOCAL);
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

    nodeManager->setupRunnerMode();
    crosshairManager->setupRunnerMode();
    oHud->setupRunnerMode();
}

void Controller::setupShooterMode() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Setting up Shooter Mode <--");

    context = CONTEXT_SHOOTER;

    nodeManager->setupShooterMode();
    crosshairManager->setupShooterMode();
    oHud->setupShooterMode();
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

void Controller::toggleMode(const Context &newContext) {
    if (newContext == CONTEXT_RUNNER && context != CONTEXT_RUNNER)
        setupRunnerMode();
    else if (newContext == CONTEXT_SHOOTER && context != CONTEXT_SHOOTER)
        setupShooterMode();
}

void Controller::setupDebugOn() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Turning Debug Mode On <--");

    debug = true;

    nodeManager->setDebugOn();
    oHud->setupDebugOn();
}

void Controller::setupDebugOff() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Turning Debug Mode Off <--");

    debug = false;

    nodeManager->setDebugOff();
    oHud->setupDebugOff();
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

bool Controller::isDebugOn() const {
    return debug;
}

}
