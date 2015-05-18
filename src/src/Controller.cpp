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

Controller::Controller() {
    go();
}

Controller::~Controller() {
    if(nodeManager)
        delete nodeManager;

    if(hud)
        delete hud;
}

void Controller::go() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: go <--");

    createRoot();
    createSceneManager();
    createOverlaySystem();
    setupResources();

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    nodeManager = new NodeManager(this);
    nodeManager->setupRunnerMode();

    hud = new HUD(this);
    hud->setupRunnerMode();
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
    Ogre::LogManager::getSingleton().logMessage("--> Creating Root <--");

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

void Controller::setupNoneMode() {
    Ogre::LogManager::getSingleton().logMessage("--> Setting up None Mode <--");
    context = CONTEXT_NONE;
    nodeManager->setupNoneMode();
    hud->setupNoneMode();
}

void Controller::toggleMode() {
    switch(context) {
    case CONTEXT_RUNNER:
        setupShooterMode();
        break;
    case CONTEXT_SHOOTER:
        setupRunnerMode();
        break;
    default:
        setupNoneMode();
    }
}

void Controller::toggleMode(const Context &newContext) {
    if (newContext == CONTEXT_NONE && context != CONTEXT_NONE)
        setupNoneMode();
    else if (newContext == CONTEXT_RUNNER && context != CONTEXT_RUNNER)
        setupRunnerMode();
    else if (newContext == CONTEXT_SHOOTER && context != CONTEXT_SHOOTER)
        setupShooterMode();
}

}
