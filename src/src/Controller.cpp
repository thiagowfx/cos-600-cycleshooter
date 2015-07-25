#include "Controller.hpp"

namespace Cycleshooter {

Context Controller::getContext() const {
    return context;
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
        std::cout << "Usage: " << argv[0] << " [options]\n"
                                             "Options\n"
                                             "    -h:      show this help\n"
                                             "    -f:      set fullscreen (1 = ON, 0 = OFF; default = ON)\n"
                                             "    -r | -s: set resolution/size (e.g. 1366x768, default = maximum possible)\n"
                     "Examples: " << argv[0] << " -r 1366x768\n"
                     "          " << argv[0] << " -f 0 -r 800x600" << std::endl;
    };

    int opt;
    while((opt = getopt(argc, argv, "f:r:s:h")) != EOF) {

        switch(opt) {
        case 'f':
            if(!(!strcmp(optarg, "0") || !strcmp(optarg, "1"))) {
                LOG_FATAL("unrecognized fullscreen parameter");
                usage();
                exit(EXIT_FAILURE);
            }
            fullscreen = atoi(optarg);
            break;

        case 's':
        case 'r':
            if (sscanf(optarg, "%dx%d", &width, &height) != 2) {
                LOG_FATAL("unrecognized resolution format");
                usage();
                exit(EXIT_FAILURE);
            }
            break;

        case 'h':
        case '?':
        default:
            usage();
            exit(EXIT_SUCCESS);
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
            LOG_FATAL("invalid fullscreen resolution specified (%d x %d). Please either set a valid resolution or disable fullscreen", width, height);
            exit(EXIT_FAILURE);
        }
    }

    go();
}

void Controller::shutdownNow(const EndGameType &endGameType) {
    shutdown = true;
    this->endGameType = endGameType;
}

LogicManager *Controller::getLogicManager() const {
    return logicManager.get();
}

CrosshairManager* Controller::getCrosshairManager() const {
    return crosshairManager.get();
}

TerrainManager* Controller::getTerrainManager() const {
    return terrainManager.get();
}

bool Controller::frameRenderingQueued(const Ogre::FrameEvent &evt) {
    // update windows, if necessary
    Ogre::WindowEventUtilities::messagePump();

    if(oWindow->isClosed())
        shutdownNow(GAME_END_MANUAL_SHUTDOWN);

    if(shutdown) {
        // sync with other threads for a clean shutdown
        waitThreads();

        return false;
    }

    // monster animations
    baseMonsterAnimation->addTime(evt.timeSinceLastFrame);
    topMonsterAnimation->addTime(evt.timeSinceLastFrame);
    swordsMonsterAnimation->addTime(evt.timeSinceLastFrame / 12.0);

    // update game logic
    logicManager->update(evt);

    if(context == CONTEXT_SHOOTER) {
        static sf::Clock clockHeartbeat;
        static int next_heartbeat_waiting_time_ms = 0;

        // (maybe) play a heartbeat sound
        if(clockHeartbeat.getElapsedTime().asMilliseconds() >= next_heartbeat_waiting_time_ms) {
            int heartRate = polar->getHeartRate();
            next_heartbeat_waiting_time_ms = (60.0 * 1000.0) / double(heartRate);
            AudioManager::instance().playHeartbeat(heartRate, HEARTBEAT_MINIMUM_ASSUMED, HEARTBEAT_MAXIMUM_ASSUMED);
            clockHeartbeat.restart();
        }

        // (maybe) randomize the crosshair
        static sf::Clock clockRandomizeCrosshair;
        if(clockRandomizeCrosshair.getElapsedTime() >= RANDOMIZE_CROSSHAIR_TIME_MS) {

            auto movementKeyPressed = []() -> bool {
                static const std::vector<sf::Keyboard::Key> movementKeys = {
                    sf::Keyboard::W,
                    sf::Keyboard::A,
                    sf::Keyboard::S,
                    sf::Keyboard::D,
                    sf::Keyboard::Left,
                    sf::Keyboard::Right,
                    sf::Keyboard::Up,
                    sf::Keyboard::Down
                };
                return InputManager::instance().isKeyPressed(movementKeys) || InputManager::instance().isJoystickLeftAxisPressed();
            };

            if(!movementKeyPressed()) {
                crosshairManager->randomizeRedCrosshair();
            }
            clockRandomizeCrosshair.restart();
        }
    }

    // update game HUD
    hud->update(evt);

    // process unbuffered keys
    static sf::Clock clockUnbuf;
    if(clockUnbuf.getElapsedTime() >= THRESHOLD_UNBUF_KEYS_MS) {
        InputManager::instance().executeActionsUnbuf(context);
        clockUnbuf.restart();
    }

    // process events (in particular, buffered keys)
    sf::Event event;
    while (sWindow->pollEvent(event)) {
        switch (event.type) {

        // window closed
        case sf::Event::Closed:
            shutdownNow(GAME_END_MANUAL_SHUTDOWN);
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
        }
    }

    return true;
}

bool Controller::getShutdown() const {
    return shutdown;
}

void Controller::incrementPlayerAmmo(){
    Ogre::Vector3 realCoord = logicManager->getPlayerNode()->getPosition();
    //std::pair<int,int> textCoord = terrainManager->getCollisionCoordinates(realCoord);
    //bool increment = terrainManager->getTerrainAt(realCoord).second;
    //std::cout << "increment" << increment<< std::endl;
    if(terrainManager->getTerrainAt(realCoord).second){
        Ogre::LogManager::getSingletonPtr()->logMessage("--> Controller: Incresgin player ammo! <--");
        logicManager->externalIncrement();
        //BIG ERROR here
        //collisionHandler->removeBullet(textCoord.first,textCoord.second);
    }
}

bool Controller::getDebug() const {
    return debug;
}

void Controller::waitThreads() const {
    LOG("Wait Threads");

    bicycleUpdater->wait();
    polarUpdater->wait();
}

void Controller::go() {
    // initialize core OGRE elements
    createSFMLWindow();
    createRoot();
    createSceneManager();
    createOverlaySystem();
    setupResources();
    setupTextures();

    if(ConfigManager::instance().getBool("Controller.countdown_enabled")) {
        startCountdown();
    }

    // initialize our objects and our game overall
    createGameElements();
    createCrosshair();
    createHud();

    // setups
    InputManager::instance().updateJoystickNumber();
    AudioManager::instance().specialAddSound(SOUND_MONSTER_CHASING, true);
    setupRunnerMode();
    setupDebugOn();
    setupKeyMappings();

    // Ogre::FrameListener <-- let's begin calling frameRenderingQueued
    oRoot->addFrameListener(this);
    gameMainLoop();
}

void Controller::createSFMLWindow() {
    LOG("Creating the SFML Window");

    sWindow = std::unique_ptr<sf::Window>(new sf::Window(sVideoMode, APPLICATION_NAME, sFullScreen, sf::ContextSettings(32, 8, 16)));
    sWindow->setIcon(CYCLESHOOTER_ICON.width, CYCLESHOOTER_ICON.height, CYCLESHOOTER_ICON.pixel_data);
    sWindow->setKeyRepeatEnabled(false);
    sWindow->setMouseCursorVisible(false);
}

void Controller::setupResources(const Ogre::String& config) {
    LOG("Setting up Resources");

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
    LOG("Setting up Textures");
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
}

void Controller::createGameElements() {
    LOG("Creating Game Elements");

    // upstream documentation: http://www.ogre3d.org/tikiwiki/Sinbad+Model
    Ogre::Entity* monsterEntity = getSceneManager()->createEntity("monsterEntity", "Sinbad.mesh");
    Ogre::SceneNode* monsterNode = getSceneManager()->getRootSceneNode()->createChildSceneNode("monsterNode", Ogre::Vector3(0.0, 0.0, +200.0));
    double monsterScale = 3.0;
    monsterNode->scale(monsterScale, monsterScale, monsterScale);
    monsterNode->attachObject(monsterEntity);

    // attention: logic manager should be created before any threads that will update it
    logicManager = std::unique_ptr<LogicManager>(new LogicManager(this));

    if(USE_REAL_BICYCLE) {
        LOG("Using the REAL BICYCLE class");
        bicycle = std::unique_ptr<AbstractBicycle>(new RealBicycle(BICYCLE_PORT.c_str()));
    }
    else {
        LOG("Using the ConstantBicycle class");
        bicycle = std::unique_ptr<AbstractBicycle>(new ConstantBicycle(1));
    }

    bicycleUpdater = std::unique_ptr<sf::Thread>(new sf::Thread([&](){
        while(!shutdown) {
            try { bicycle->updateSpeed(); }
            catch (...) { Ogre::LogManager::getSingleton().logMessage("WARNING: Controller (bicycle thread): exception caught", Ogre::LML_CRITICAL); }
            sf::sleep(BICYCLE_SLEEP_TIME_MS);
        }
    }));
    bicycleUpdater->launch();

    if(USE_REAL_POLAR) {
        LOG("Using the REAL POLAR class");
        polar = std::unique_ptr<AbstractPolar>(new RealPolar(POLAR_PORT.c_str()));
    }
    else {
        LOG("Using a polar test class");
        polar = std::unique_ptr<AbstractPolar>(new ConstantPolar((HEARTBEAT_MINIMUM_ASSUMED + HEARTBEAT_MAXIMUM_ASSUMED) / 2));
    }

    polarUpdater = std::unique_ptr<sf::Thread>(new sf::Thread([&](){
        while(!shutdown) {
            try { polar->updateHeartRate(); }
            catch (...) { Ogre::LogManager::getSingleton().logMessage("WARNING: Controller (polar thread): exception caught", Ogre::LML_CRITICAL); }
            sf::sleep(POLAR_SLEEP_TIME_MS);
        }
    }));
    polarUpdater->launch();

    // to use a material, the resource group must be initialized
    terrainManager = std::unique_ptr<TerrainManager>(new TerrainManager(oSceneManager,"racecircuit.png"));
    terrainManager->createTerrain();
    //terrainManager->sampleCollisionTransformation();

    // create a skybox
    getSceneManager()->setSkyDome(true, "Cycleshooter/CloudySky");

    createAnimations();
    createSwords();

    getSceneManager()->setAmbientLight(Ogre::ColourValue(0.6, 0.6, 0.6));

    getSceneManager()->createLight("mainLight")->setPosition(20.0, 80.0, 50.0);
    getSceneManager()->createLight("auxLight1")->setPosition(+100.0, +100.0, +100.0);
    getSceneManager()->createLight("auxLight2")->setPosition(-100.0, +50.0, -100.0);
}

void Controller::createCrosshair() {
    LOG("Creating Crosshair");
    crosshairManager = std::unique_ptr<CrosshairManager>(new CrosshairManager());
}

void Controller::createHud() {
    LOG("Creating HUD");
    hud = std::unique_ptr<HUD>(new HUD(this));
}

void Controller::createAnimations() {
    Ogre::Entity* monsterEntity = getSceneManager()->getEntity("monsterEntity");

    monsterEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

    baseMonsterAnimation = monsterEntity->getAnimationState("RunBase");
    topMonsterAnimation = monsterEntity->getAnimationState("RunTop");
    swordsMonsterAnimation = monsterEntity->getAnimationState("DrawSwords");

    std::vector<Ogre::AnimationState*> animations = {
        baseMonsterAnimation,
        topMonsterAnimation,
        swordsMonsterAnimation
    };

    // enable both of them and set them to loop
    for (int i = 0; i < (int) animations.size(); ++i) {
        animations[i]->setLoop(true);
        animations[i]->setEnabled(true);
    }
}

void Controller::createSwords() {
    Ogre::Entity* monsterEntity = getSceneManager()->getEntity("monsterEntity");

    // attach the two swords to sheath
    // upstream documentation: https://bitbucket.org/sinbad/ogre/src/78cf231243e2/Samples/Character/?at=default
    Ogre::Entity* swords[] = {
        getSceneManager()->createEntity("SinbadSword1", "Sword.mesh"),
        getSceneManager()->createEntity("SinbadSword2", "Sword.mesh"),
        getSceneManager()->createEntity("SinbadSword3", "Sword.mesh"),
        getSceneManager()->createEntity("SinbadSword4", "Sword.mesh")
    };

    Ogre::String swordsPosition[] = {
        "Sheath.L",
        "Sheath.R",
        "Handle.L",
        "Handle.R"
    };

    int swordsLength = sizeof(swords) / sizeof(Ogre::Entity*);

    for (int i = 0; i < swordsLength; ++i) {
        monsterEntity->attachObjectToBone(swordsPosition[i], swords[i]);
    }

    // setting Trails to the sword animation
    Ogre::NameValuePairList params;
    params["numberOfChains"] = "2";
    params["maxElements"] = "80";

    Ogre::RibbonTrail* swordTrail = static_cast<Ogre::RibbonTrail*>(getSceneManager()->createMovableObject("RibbonTrail", &params));
    swordTrail->setMaterialName("Cycleshooter/LightRibbonTrail");
    swordTrail->setTrailLength(20);
    swordTrail->setVisible(true);
    getSceneManager()->getRootSceneNode()->attachObject(swordTrail);

    for (int i = 0; i < 2; ++i) {
        swordTrail->setInitialColour(i, 1, 0.8, 0);
        swordTrail->setColourChange(i, 0.75, 1.25, 1.25, 1.25);
        swordTrail->setWidthChange(i, 1);
        swordTrail->setInitialWidth(i, 0.5);
    }

    swordTrail->addNode(swords[2]->getParentNode());
    swordTrail->addNode(swords[3]->getParentNode());
}

void Controller::setupKeyMappings() {
    LOG("Setting up mappings");

    /*
     * Runner mode mappings;
     */
    InputManager::instance().addKeysUnbuf({sf::Keyboard::W,
                                           sf::Keyboard::Up}, CONTEXT_RUNNER, [&]{
        // logicManager->getPlayerNode()->translate(Ogre::Vector3(0.0, 0.0, -10.0), Ogre::SceneNode::TS_LOCAL);
        bicycle->changeSpeed(BICYCLE_SPEED_CHANGE);
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::S,
                                           sf::Keyboard::Down}, CONTEXT_RUNNER, [&]{
        // logicManager->getPlayerNode()->translate(Ogre::Vector3(0.0, 0.0, +10.0), Ogre::SceneNode::TS_LOCAL);
        bicycle->changeSpeed(-BICYCLE_SPEED_CHANGE);
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::A,
                                           sf::Keyboard::Left}, CONTEXT_RUNNER, [&]{
        logicManager->getPlayerNode()->yaw(Ogre::Degree(+10.0));
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::D,
                                           sf::Keyboard::Right}, CONTEXT_RUNNER, [&]{
        logicManager->getPlayerNode()->yaw(Ogre::Degree(-10.0));
    });

    InputManager::instance().addKey(sf::Keyboard::Q, CONTEXT_RUNNER, [&]{
        bicycle->changeFriction(-BICYCLE_FRICTION_CHANGE);
    });

    InputManager::instance().addKey(sf::Keyboard::E, CONTEXT_RUNNER, [&]{
        bicycle->changeFriction(BICYCLE_FRICTION_CHANGE);
    });

    /*
     * Shooter mode mappings.
     */
    InputManager::instance().addKeysUnbuf({sf::Keyboard::A,
                                           sf::Keyboard::Left}, CONTEXT_SHOOTER, [&]{
        crosshairManager->scrollVirtualCrosshair(polar->getHeartRate(), -1.0, 0.0);
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::D,
                                           sf::Keyboard::Right}, CONTEXT_SHOOTER, [&]{
        crosshairManager->scrollVirtualCrosshair(polar->getHeartRate(), +1.0, 0.0);
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::W,
                                           sf::Keyboard::Up}, CONTEXT_SHOOTER, [&]{
        crosshairManager->scrollVirtualCrosshair(polar->getHeartRate(), 0.0, +1.0);
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::S,
                                           sf::Keyboard::Down}, CONTEXT_SHOOTER, [&]{
        crosshairManager->scrollVirtualCrosshair(polar->getHeartRate(), 0.0, -1.0);
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

    InputManager::instance().addKey(sf::Keyboard::LBracket, [&]{
        polar->changePeaks(-POLAR_PEAK_CHANGE);
    });

    InputManager::instance().addKey(sf::Keyboard::RBracket, [&]{
        polar->changePeaks(POLAR_PEAK_CHANGE);
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
        shutdownNow(GAME_END_MANUAL_SHUTDOWN);
    });

    /*
     * Joystick mappings.
     */
    InputManager::instance().addJoystickAxisUnbuf(sf::Joystick::X, CONTEXT_SHOOTER, [&](float f) {
        crosshairManager->scrollVirtualCrosshair(polar->getHeartRate(), +1.0 * (f / 100.0), 0.0);
    });

    InputManager::instance().addJoystickAxisUnbuf(sf::Joystick::Y, CONTEXT_SHOOTER, [&](float f) {
        crosshairManager->scrollVirtualCrosshair(polar->getHeartRate(), 0.0, +1.0 * (f / 100.0));
    });

    InputManager::instance().addJoystickButtons({0}, CONTEXT_SHOOTER, [&]() {
        logicManager->shoot();
    });

    InputManager::instance().addJoystickButtons({1}, [&]() {
        toggleMode();
    });
}

AbstractPolar* Controller::getPolar() const {
    return polar.get();
}

void Controller::gameMainLoop() {
    LOG("Entering the Game Main Loop");

    gameStartClock = std::chrono::high_resolution_clock::now();

    while(!shutdown) {
        // update rendering
        oRoot->renderOneFrame();
    }

    waitThreads();
    doGameEnd();
}

void Controller::doGameEnd() {
    std::string totalGameTime = getElapsedTimeAsString();

    bool gameVictory = isGameVictory(endGameType);

    LOG("Stopping game music");
    AudioManager::instance().specialRemoveSound(SOUND_MONSTER_CHASING);
    AudioManager::instance().stopMusic();

    LOG("Unmapping all keys");
    InputManager::instance().reset();

    LOG("Removing HUD");
    hud.reset(nullptr);

    if(endGameType == GAME_END_CAUGHT_BY_MONSTER) {
        AudioManager::instance().playSound(SOUND_PLAYER_DEATH);
        sf::sleep(AudioManager::instance().getSoundDuration(SOUND_PLAYER_DEATH));
    }
    else if(endGameType == GAME_END_MONSTER_KILLED){
        AudioManager::instance().playSound(SOUND_MONSTER_DEATH);
        AudioManager::instance().playSound(SOUND_MONSTER_DEATH_DOTA);
        sf::Time soundDurationA = AudioManager::instance().getSoundDuration(SOUND_MONSTER_DEATH);
        sf::Time soundDurationB = AudioManager::instance().getSoundDuration(SOUND_MONSTER_DEATH_DOTA);
        sf::sleep(std::max(soundDurationA,soundDurationB));
    }
    else if(endGameType == GAME_END_WALL_CRASH){
        AudioManager::instance().playSound(SOUND_WALL_CRASH);
        sf::sleep(AudioManager::instance().getSoundDuration(SOUND_WALL_CRASH));
    }

    LOG("Clearing the scene");
    oSceneManager->clearScene();

    LOG("Removing all viewports");
    oWindow->removeAllViewports();

    LOG("Creating the final scene");

    // create a background, for cleaning purposes
    Ogre::Camera* endCamera = oSceneManager->createCamera("endCamera");
    Ogre::Viewport* endViewport = oWindow->addViewport(endCamera);
    endViewport->setBackgroundColour(gameVictory ? Ogre::ColourValue::Green : Ogre::ColourValue::Red);

    // render final game image
    Ogre::OverlayManager::getSingleton().getByName(gameVictory ? "Cycleshooter/GameVictory" : "Cycleshooter/GameOver")->show();
    oWindow->update();

    Soundname endSound = gameVictory ? SOUND_GAME_VICTORY : SOUND_GAME_LOSS;
    AudioManager::instance().playSound(endSound);

    std::ofstream ofs(ConfigManager::instance().getStr("Controller.statistics_file"), std::ios_base::app | std::ios_base::out);

    ofs << "===== SESSION STARTS: " << gameStartClock << std::endl;

    polar->printStatistics(ofs);
    bicycle->printStatistics(ofs);

    ofs << "* Other Statistics\n"
           "- Total game time: " << totalGameTime << std::endl <<
           "- End Game type: " << endGameTypeToString(endGameType) << std::endl;

    ofs << "===== SESSION ENDS: " << std::chrono::system_clock::now() << std::endl << std::endl;

    sf::sleep(AudioManager::instance().getSoundDuration(endSound));
}

void Controller::startCountdown() {
    LOG("Starting countdown");

    oWindow->addViewport(oSceneManager->createCamera("countdownCamera"), 0);
    int COUNTDOWN_TIME_MS = ConfigManager::instance().getInt("Controller.countdown_time_ms");

    std::vector<Ogre::String> countdowns = {
        "Cycleshooter/Countdown3",
        "Cycleshooter/Countdown2",
        "Cycleshooter/Countdown1",
        "Cycleshooter/Countdown0"
    };

    for (auto countdown : countdowns) {
        Ogre::OverlayManager::getSingleton().getByName(countdown)->show();
        oWindow->update();
        AudioManager::instance().playSound(SOUND_COUNTDOWN);
        sf::sleep(sf::milliseconds(COUNTDOWN_TIME_MS));
        Ogre::OverlayManager::getSingleton().getByName(countdown)->hide();
    }

    oWindow->removeViewport(0);
    oSceneManager->destroyCamera("countdownCamera");
}

void Controller::createRoot() {
    LOG("Creating Ogre Root");
    oRoot = new Ogre::Root();

    // create rendering system, but don't initialise it / the main window
    oRoot->setRenderSystem(oRoot->getAvailableRenderers().at(0));
    oRoot->initialise(false);

    LOG("Ogre::Root object has been created and initialized with the default Render System (%s)", oRoot->getAvailableRenderers().at(0)->getName().c_str());
    Ogre::NameValuePairList misc = {{"currentGLContext", "true"}};

    // create a render window
    // note: window title and size are not important here, so we use blank (dummy) values for them
    oWindow = oRoot->createRenderWindow("", 0, 0, false, &misc);
    oWindow->setVisible(true);

    LOG("Render Window has been (manually) created");
}

void Controller::createSceneManager() {
    LOG("Creating the Scene Manager");
    oSceneManager = oRoot->createSceneManager("OctreeSceneManager", "sceneManager");
    // alt: oSceneManager = oRoot->createSceneManager(Ogre::ST_GENERIC, "sceneManager");
}

void Controller::createOverlaySystem() {
    LOG("Creating the Overlay System");
    oOverlaySystem = new Ogre::OverlaySystem();
    oSceneManager->addRenderQueueListener(oOverlaySystem);
}

void Controller::setupRunnerMode() {
    LOG("Setting up Runner Mode");
    context = CONTEXT_RUNNER;

    logicManager->setupRunnerMode();
    crosshairManager->setupRunnerMode();
    hud->setupRunnerMode();
    AudioManager::instance().playMusic(MUSIC_RUNNER);
    AudioManager::instance().specialGetSound(SOUND_MONSTER_CHASING)->play();
}

void Controller::setupShooterMode() {
    LOG("Setting up Shooter Mode");
    context = CONTEXT_SHOOTER;

    logicManager->setupShooterMode();
    crosshairManager->setupShooterMode();
    hud->setupShooterMode();
    AudioManager::instance().playMusic(MUSIC_SHOOTER);
    AudioManager::instance().specialGetSound(SOUND_MONSTER_CHASING)->pause();
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
    LOG("Turning Debug Mode On");
    debug = true;

    logicManager->setDebugOn();
    hud->setDebug(true);
}

void Controller::setupDebugOff() {
    LOG("Turning Debug Mode Off");
    debug = false;

    logicManager->setDebugOff();
    hud->setDebug(false);
}

void Controller::toggleDebug() {
    debug ? setupDebugOff() : setupDebugOn();
}

std::string Controller::getElapsedTimeAsString() const {
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - gameStartClock).count();

    int min = static_cast<int>(duration) / 60;
    int sec = static_cast<int>(duration) % 60;

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << min << ":" << std::setfill('0') << std::setw(2) << sec;
    return ss.str();
}

AbstractBicycle* Controller::getBicycle() const {
    return bicycle.get();
}

}
