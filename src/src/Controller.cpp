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
    return oRoot->getAutoCreatedWindow();
}

Ogre::SceneManager *Controller::getSceneManager() const {
    return oSceneManager;
}

Ogre::OverlaySystem *Controller::getOverlaySystem() const {
    return oOverlaySystem;
}

HUD *Controller::getHud() const {
    return hud;
}

void Controller::setHud(HUD *value) {
    hud = value;
}

Controller::Controller() {
    go();
}

Controller::~Controller() {
    if(nodeManager)
        delete nodeManager;
}

Debug Controller::getDebug() const {
    return debug;
}

void Controller::go() {
    createRoot();
    createSceneManager();
    createOverlaySystem();
    setupResources();

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    context = CONTEXT_RUNNER;

    nodeManager = new NodeManager(this);
    nodeManager->setupRunnerMode();
}

void Controller::setupResources() {
    Ogre::LogManager::getSingleton().logMessage("--> Setting up Resources <--");

    Ogre::ConfigFile cf;
    cf.load(RESOURCES_CONFIG);

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

void Controller::createRoot() {
    oRoot = new Ogre::Root();

    // alternatively, use ->restoreConfig() to load saved settings
    if(!oRoot->showConfigDialog()) {
        return;
    }

    // automatically create a window
    oRoot->initialise(true, RENDER_WINDOW_NAME);
}

void Controller::createSceneManager() {
    Ogre::LogManager::getSingleton().logMessage("--> Creating Scene Manager <--");
    oSceneManager = oRoot->createSceneManager(Ogre::ST_GENERIC);
}

void Controller::createOverlaySystem() {
    Ogre::LogManager::getSingleton().logMessage("--> Creating Overlay System <--");
    oOverlaySystem = new Ogre::OverlaySystem();
    oSceneManager->addRenderQueueListener(oOverlaySystem);
}

void Controller::setupRunnerMode() {
    Ogre::LogManager::getSingleton().logMessage("--> Setting up Runner Mode <--");
    context = CONTEXT_RUNNER;
    nodeManager->setupRunnerMode();
    hud->setupRunnerMode();
}

void Controller::setupShooterMode() {
    Ogre::LogManager::getSingleton().logMessage("--> Setting up Shooter Mode <--");
    context = CONTEXT_SHOOTER;
    nodeManager->setupShooterMode();
    hud->setupShooterMode();
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
    Ogre::LogManager::getSingleton().logMessage("--> Turning Debug On <--");
    debug = DEBUG_ON;
    nodeManager->setDebugOn();
    hud->setupDebugOn();
}

void Controller::setupDebugOff() {
    Ogre::LogManager::getSingleton().logMessage("--> Turning Debug Off <--");
    debug = DEBUG_OFF;
    nodeManager->setDebugOff();
    hud->setupDebugOff();
}

void Controller::toggleDebug() {
    switch(debug) {
    case DEBUG_ON:
        setupDebugOff();
        break;
    case DEBUG_OFF:
        setupDebugOn();
        break;
    }
}

void Controller::toggleDebug(const Debug &newDebug) {
    if(newDebug == DEBUG_ON && debug != DEBUG_ON)
        setupDebugOn();
    else if (newDebug == DEBUG_OFF && debug != DEBUG_OFF)
        setupDebugOff();
}

}
