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
                usage();
                LOG_FATAL("unrecognized fullscreen parameter");
            }
            fullscreen = atoi(optarg);
            break;

        case 's':
        case 'r':
            if (sscanf(optarg, "%dx%d", &width, &height) != 2) {
                usage();
                LOG_FATAL("unrecognized resolution format");
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

    // pathManager updates
    pathManager->monsterPathUpdate(evt.timeSinceLastFrame, logicManager->getPlayerPosition(), logicManager->getMonsterPosition());

    logicManager->updateMonster(pathManager->getMonsterTangent(), pathManager->getMonsterNextPosition());

    // monster animations
    baseMonsterAnimation->addTime(evt.timeSinceLastFrame);
    topMonsterAnimation->addTime(evt.timeSinceLastFrame);
    swordsMonsterAnimation->addTime(evt.timeSinceLastFrame / 10.0);

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

        // randomize the crosshair if no movement key is pressed
        static sf::Clock clockRandomizeCrosshair;
        if(clockRandomizeCrosshair.getElapsedTime() >= RANDOMIZE_CROSSHAIR_TIME_MS) {

            if(!(InputManager::instance().isMovementKeyPressed() || InputManager::instance().isJoystickMovementAxisPressed())) {
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
        InputManager::instance().executeKeyboardActionsUnbuf(context);
        clockUnbuf.restart();
    }

    static sf::Clock dumpLogClock;
    sf::Time DUMP_LOG_TIME_MS = sf::milliseconds(ConfigManager::instance().getInt("Controller.dump_log_time_ms"));
    if(dumpLogClock.getElapsedTime() >= DUMP_LOG_TIME_MS) {
        dumpLog();
        dumpLogClock.restart();
    }

    // camera rotation stuff
    static sf::Clock rotationUnbufClock;
    sf::Time ROTATION_UNBUF_TIME_MS = sf::milliseconds(ConfigManager::instance().getInt("Controller.threshold_rotation_keys_ms"));
    if(rotationUnbufClock.getElapsedTime() >= ROTATION_UNBUF_TIME_MS) {
        InputManager::instance().executeJoystickActionsUnbuf(context);
        InputManager::instance().executeActionsRotationUnbuf(context);
        rotationUnbufClock.restart();
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

        default:
            break;
        }
    }

    return true;
}

bool Controller::getShutdown() const {
    return shutdown;
}

bool Controller::getDebug() const {
    return debug;
}

std::string Controller::chronoToDateString(decltype(std::chrono::system_clock::now()) clock) const {
    /**
     * @brief CURRENT_DATE_FORMAT The format of the date and time for the dump log.
     */
    const char* CURRENT_DATE_FORMAT = ConfigManager::instance().getStr("Controller.current_date_format").c_str();
    const int BUFFER_SIZE = 32;
    std::time_t t = std::chrono::system_clock::to_time_t(clock);
    char s[BUFFER_SIZE];
    std::strftime(s, sizeof(s), CURRENT_DATE_FORMAT, std::localtime(&t));

    return s;
}

std::string Controller::generateCurrentDate() const {
    return chronoToDateString(std::chrono::system_clock::now());
}

void Controller::dumpLog() {
    std::stringstream ss;
    ss << getPolar()->getHeartRate() << ","
       << getBicycle()->getRpmSpeed() << ","
       << getBicycle()->getFriction() << ","
       << getLogicManager()->getPlayerAmmo() << ","
       << getLogicManager()->getDistanceToMonster() << ","
       << getLogicManager()->getMonsterHealth();
    Ogre::LogManager::getSingleton().getLog(DUMP_LOG_FILENAME)->logMessage(ss.str());
}

void Controller::initializeDumpLog() {
    LOG("Initializing the dump log");
    DUMP_LOG_FILENAME = DUMPS_DIRECTORY + generateCurrentDate() + ".csv";
    Ogre::Log* dumpLog = Ogre::LogManager::getSingleton().createLog(DUMP_LOG_FILENAME, false, false);
    dumpLog->setTimeStampEnabled(false);
    dumpLog->logMessage("polar,rpm_speed,friction,ammo,dist_to_monster,monster_health");
}

void Controller::waitThreads() const {
    LOG("Waiting for Threads");

    bicycleUpdater->wait();
    polarUpdater->wait();
}

void Controller::go() {
    srand(time(NULL));

    // initialize core OGRE elements
    createSFMLWindow();
    createRoot();
    Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_LOW);
    createSceneManager();
    createOverlaySystem();
    setupResources();
    setupTextures();

    if(ConfigManager::instance().getBool("Release.game_release")) {
        doCountdown();
    }

    // initialize our objects and our game overall
    createGameElements();
    createCrosshair();
    createHud();

    // setups
    InputManager::instance().updateJoystickNumber();
    AudioManager::instance().specialAddSound(SOUND_MONSTER_CHASING, true);
    setupRunnerMode();
    setDebug(!ConfigManager::instance().getBool("Release.game_release"));
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

    //Creating the path manager
    pathManager = std::unique_ptr<PathManager>(new PathManager("track1.txt"));
    //poligonalPathManager = std::unique_ptr<PoligonalPathManager>(new PoligonalPathManager("track1.txt"));
    unsigned monsterInitialIndex = 0;
    Ogre::Vector3 monsterInitialPosition = pathManager->getPoint(monsterInitialIndex);
    Ogre::Vector3 monsterInitialLookAt = pathManager->getPoint(monsterInitialIndex + 1) - pathManager->getPoint(monsterInitialIndex);
    // upstream documentation: http://www.ogre3d.org/tikiwiki/Sinbad+Model
    Ogre::Entity* monsterEntity = getSceneManager()->createEntity("monsterEntity", "Sinbad.mesh");
    Ogre::SceneNode* monsterNode = getSceneManager()->getRootSceneNode()->createChildSceneNode("monsterNode", monsterInitialPosition);
    double MONSTER_SCALE_SIZE = ConfigManager::instance().getDouble("Controller.monster_scale_size");
    monsterNode->scale(MONSTER_SCALE_SIZE, MONSTER_SCALE_SIZE, MONSTER_SCALE_SIZE);
    monsterNode->attachObject(monsterEntity);
    monsterNode->setDirection(monsterInitialLookAt);


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
    //terrainManager->sampleCollisionTransformation();

    // create a skybox
    getSceneManager()->setSkyDome(true, "Cycleshooter/NightySky", 3, 4);

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

    if(!ConfigManager::instance().getBool("Release.game_release")) {

#define bicycleSpeedChange(signal_sensibility) bicycle->changeSpeed(static_cast<float>(signal_sensibility) * BICYCLE_SPEED_CHANGE);
#define bicycleRotationChange(signal_sensibility)\
    Ogre::Degree angle = Ogre::Degree(ConfigManager::instance().getDouble("Controller.camera_rotation_angle_per_frame"));\
    logicManager->rotateCamera(static_cast<float>(signal_sensibility) * angle);
#define scrollCrosshair(x_sensibility, y_sensibility)\
    crosshairManager->scrollVirtualCrosshair(polar->getHeartRate(), x_sensibility, y_sensibility);

        /*
     * Runner mode mappings;
     */
        InputManager::instance().addKeysUnbuf({sf::Keyboard::W,
                                               sf::Keyboard::Up}, CONTEXT_RUNNER, [&]{
            bicycleSpeedChange(+1);

        });

        InputManager::instance().addKeysUnbuf({sf::Keyboard::S,
                                               sf::Keyboard::Down}, CONTEXT_RUNNER, [&]{
            bicycleSpeedChange(-1);
        });

        /*
     * Rotation Mapping
     * WITH THIS APPROACH ROTATION WORKS WITH ITS OWN KEYBOARD MAPPING (NO MAXIMUM ANGLE)
     */
        InputManager::instance().addKeysRotationUnbuf({sf::Keyboard::A,
                                                       sf::Keyboard::Left}, CONTEXT_RUNNER, [&]{
            bicycleRotationChange(+1);
        });

        InputManager::instance().addKeysRotationUnbuf({sf::Keyboard::D,
                                                       sf::Keyboard::Right}, CONTEXT_RUNNER, [&]{
            bicycleRotationChange(-1);
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
            scrollCrosshair(-1.0, 0.0);
        });

        InputManager::instance().addKeysUnbuf({sf::Keyboard::D,
                                               sf::Keyboard::Right}, CONTEXT_SHOOTER, [&]{
            scrollCrosshair(+1.0, 0.0);
        });

        InputManager::instance().addKeysUnbuf({sf::Keyboard::W,
                                               sf::Keyboard::Up}, CONTEXT_SHOOTER, [&]{
            scrollCrosshair(0.0, +1.0);
        });

        InputManager::instance().addKeysUnbuf({sf::Keyboard::S,
                                               sf::Keyboard::Down}, CONTEXT_SHOOTER, [&]{
            scrollCrosshair(0.0, -1.0);
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
    }

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

    /*
     * Xbox Controller
     * 8 axes:
     *
     * X = horizontal, left pad
     * Y = vertical, left pad
     * U = horizontal, right pad
     * V = vertical, right pad
     * PovX = horizontal, left directional controls
     * PovY = vertical, right directional controls
     * Z = "L2"
     * R = "R2"
     *
     */
    InputManager::instance().addJoystickAxisUnbuf({sf::Joystick::X,
                                                   sf::Joystick::U,
                                                   sf::Joystick::PovX},
                                                  CONTEXT_RUNNER, [&](float f) {
        bicycleRotationChange(-f / 100.0);
    });

    if(!ConfigManager::instance().getBool("Release.game_release")) {
        InputManager::instance().addJoystickAxisUnbuf({sf::Joystick::Y,
                                                       sf::Joystick::V,
                                                       sf::Joystick::PovY},
                                                      CONTEXT_RUNNER, [&](float f) {
            // note: Y axis is reversed
            bicycleSpeedChange(-f / 100.0);
        });
    }

    InputManager::instance().addJoystickAxisUnbuf({sf::Joystick::X,
                                                   sf::Joystick::U,
                                                   sf::Joystick::PovX},
                                                  CONTEXT_SHOOTER, [&](float f) {
        scrollCrosshair(+f / 100.0, 0.0);
    });

    InputManager::instance().addJoystickAxisUnbuf({sf::Joystick::Y,
                                                   sf::Joystick::V,
                                                   sf::Joystick::PovY},
                                                  CONTEXT_SHOOTER, [&](float f) {
        scrollCrosshair(0.0, -f / 100.0);
    });

    /*
     * Xbox Controller
     * 11 buttons:
     *
     * 0 = A
     * 1 = B
     * 2 = X
     * 3 = Y
     * 4 = "R1"
     * 5 = "L1"
     * 6 = "select"
     * 7 = "start"
     * 8 = "xbox"
     * 9 = "L3" (normal state: -1.0)
     * 10 = "R3" (normal state: -1.0)
     *
     */
    InputManager::instance().addJoystickButtons({0, 1, 2, 5}, CONTEXT_SHOOTER, [&]() {
        logicManager->shoot();
    });

    InputManager::instance().addJoystickAxisUnbuf({sf::Joystick::Z,
                                                   sf::Joystick::R},
                                                  CONTEXT_SHOOTER, [&](float f) {
        if(f > -1.0) {
            logicManager->shoot();
        }
    });

    InputManager::instance().addJoystickButtons({3, 4, 6, 8}, [&]() {
        toggleMode();
    });
}

PathManager* Controller::getPathManager() const {
    return pathManager.get();
}

AbstractPolar* Controller::getPolar() const {
    return polar.get();
}

void Controller::gameMainLoop() {
    LOG("Entering the Game Main Loop");
    initializeDumpLog();
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
        AudioManager::instance().playSound(SOUND_MONSTER_LAUGH);
        sf::Time soundDurationA = AudioManager::instance().getSoundDuration(SOUND_PLAYER_DEATH);
        sf::Time soundDurationB = AudioManager::instance().getSoundDuration(SOUND_MONSTER_LAUGH);
        sf::sleep(std::max(soundDurationA,soundDurationB));
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

    if(ConfigManager::instance().getBool("Release.game_release")) {
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
        sf::sleep(AudioManager::instance().getSoundDuration(endSound));
    }

    doSaveStats(ConfigManager::instance().getStr("Controller.statistics_file").c_str(), totalGameTime.c_str());
}

void Controller::doSaveStats(const char* file, const char* totalGameTime) {
    std::ofstream ofs(file, std::ios_base::app | std::ios_base::out);

    ofs << "===== SESSION STARTS: " << chronoToDateString(gameStartClock) << std::endl;

    polar->printStatistics(ofs);
    bicycle->printStatistics(ofs);

    ofs << "* Other Statistics" << std::endl <<
           "- Total game time: " << totalGameTime << std::endl <<
           "- End Game type: " << EndGameTypeToString(endGameType) << std::endl;

    ofs << "===== SESSION ENDS: " << generateCurrentDate() << std::endl << std::endl;
}

void Controller::doCountdown() {
    LOG("Starting countdown");

    AudioManager::instance().playSound(SOUND_GAME_BATTLE_PREPARE_DOTA);
    sf::sleep(AudioManager::instance().getSoundDuration(SOUND_GAME_BATTLE_PREPARE_DOTA));

    oWindow->addViewport(oSceneManager->createCamera("countdownCamera"), 0);

    std::vector<Ogre::String> countdowns = {
        "Cycleshooter/Countdown3",
        "Cycleshooter/Countdown2",
        "Cycleshooter/Countdown1",
        "Cycleshooter/Countdown0"
    };

    for (const auto& countdown : countdowns) {
        Ogre::OverlayManager::getSingleton().getByName(countdown)->show();
        oWindow->update();
        AudioManager::instance().playSound(SOUND_COUNTDOWN);
        sf::sleep(sf::milliseconds(ConfigManager::instance().getInt("Controller.countdown_time_ms")));
        Ogre::OverlayManager::getSingleton().getByName(countdown)->hide();
    }

    oWindow->removeViewport(0);
    oSceneManager->destroyCamera("countdownCamera");

    AudioManager::instance().playSound(SOUND_GAME_BATTLE_BEGINS_DOTA);
    sf::sleep(AudioManager::instance().getSoundDuration(SOUND_GAME_BATTLE_PREPARE_DOTA));
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
    Ogre::Root::getSingleton().setFrameSmoothingPeriod(1.0 / 60.0);

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

void Controller::setDebug(bool debug) {
    if(debug) {
        LOG("Turning Debug Mode ON");
    }
    else {
        LOG("Turning Debug Mode OFF");
    }

    this->debug = debug;

    logicManager->setDebug(debug);
    hud->setDebug(debug);
    pathManager->setDebug(debug);
    //poligonalPathManager->setDebug(debug);
}

void Controller::toggleDebug() {
    setDebug(!debug);
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
