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

Controller::Controller() {
    go();
}

Controller::~Controller() {
    if(oHud)
        delete oHud;

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

    // TODO: update logic[manager] here somewhere

    oHud->update(evt);

    // process unbuffered keys
    if(clockUnbuf.getElapsedTime() >= THRESHOLD_UNBUF_KEYS) {
        InputManager::instance().executeActionsUnbuf(context);
        clockUnbuf.restart();
    }

    // process events (in particular, buffered keys)
    sf::Event event;

    // TODO: check if there is the possibility of an infinite event loop
    // while there are pending events...
    while (sWindow->pollEvent(event)) {

        // check the type of the event...
        switch (event.type) {

        // window closed
        case sf::Event::Closed:
            sWindow->close();
            shutdownNow();
            break;

            // TODO: resize event (adjust viewport)

            // key pressed
        case sf::Event::KeyPressed:
            InputManager::instance().executeKeyAction(event.key.code, context);
            break;

        case sf::Event::JoystickButtonPressed:
            InputManager::instance().executeJoystickButtonAction(event.joystickButton.button, context);
            break;

            // TODO: add mouse events

            // don't process other types of events
        default:
            break;
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
    createHud();

    // setups
    InputManager::instance().detectJoystick();
    setupMappings();
    // TODO: setupRunnerMode() (?) -- or each element individually?
    setupDebugOn();

    // Ogre::FrameListener <-- let's begin calling frameRenderingQueued
    oRoot->addFrameListener(this);
    gameMainLoop();
}

void Controller::createSFMLWindow() {
    std::cout << "--> Controller: Creating the SFML Window <--" << std::endl;

    // TODO: refine those peculiarities
    // TODO: if/else fullscreen (user configurable)
    // TODO: if/else fullscreen resolution (user configurable)
    sWindow = new sf::Window(sf::VideoMode::getFullscreenModes()[0], APPLICATION_NAME, sf::Style::Default, sf::ContextSettings(32));
    sWindow->setIcon(cycleshooter_icon.width, cycleshooter_icon.height, cycleshooter_icon.pixel_data);
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
    logicManager = new LogicManager();

    polar = new RandomPolar();
    polarUpdater = new sf::Thread(&Controller::polarUpdaterFunction, this);
    polarUpdater->launch();

    nodeManager = new NodeManager(this);
    nodeManager->setupRunnerMode();

    // to use a material, the resource group must be initialized
    terrainManager = new TerrainManager(oSceneManager);
    terrainManager->createTerrain();

    // starting collision handler after terrain initialization
    collisionHandler = new CollisionHandler("racecircuit.png");
    collisionHandler->loadImages();
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

void Controller::createHud() {
    Ogre::LogManager::getSingletonPtr()->logMessage("--> Controller: Creating HUD <--");

    oHud = new HUD(this);
    oHud->setHelpPanel({"1", "2"},{"ToggleMode", "ToggleDebug"});
    oHud->setupRunnerMode();
}

void Controller::setupMappings() {
    // refresh (reload) all textures
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

    InputManager::instance().addKey(sf::Keyboard::Num1, [&]{
        toggleMode();
    });

    InputManager::instance().addKey(sf::Keyboard::Num2, [&]{
        toggleDebug();
    });

    //
    // joystick
    //
    InputManager::instance().addJoystickAxisUnbuf(sf::Joystick::X, CONTEXT_RUNNER, [&](float f){
        getNodeManager()->getParentPlayerSceneNode()->yaw(Ogre::Degree(-10 * f / 100.0));
    });

    InputManager::instance().addJoystickAxisUnbuf(sf::Joystick::Y, CONTEXT_RUNNER, [&](float f){
        getNodeManager()->getParentPlayerSceneNode()->translate(Ogre::Vector3(0.0, 0.0, 10.0 * f / 100.0), Ogre::SceneNode::TS_LOCAL);
    });

    InputManager::instance().addJoystickButton(0, [&]{
        toggleMode();
    });
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
    oHud->setupRunnerMode();
}

void Controller::setupShooterMode() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Setting up Shooter Mode <--");

    context = CONTEXT_SHOOTER;

    nodeManager->setupShooterMode();
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
