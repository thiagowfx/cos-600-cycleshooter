#include "Controller.hpp"

namespace Cycleshooter {

Context Controller::getContext() const {
    return context;
}

void Controller::setContext(const Context &value) {
    context = value;
}

NodeManager *Controller::getNodeManager() const {
    return nodeManager;
}

void Controller::setNodeManager(NodeManager *value) {
    nodeManager = value;
}

Ogre::Root *Controller::getRoot() const {
    return oRoot;
}

void Controller::setRoot(Ogre::Root *value) {
    oRoot = value;
}

Ogre::SceneManager *Controller::getSceneManager() const {
    return oSceneManager;
}

void Controller::setSceneManager(Ogre::SceneManager *value) {
    oSceneManager = value;
}

Ogre::OverlaySystem *Controller::getOverlaySystem() const {
    return oOverlaySystem;
}

void Controller::setOverlaySystem(Ogre::OverlaySystem *value) {
    oOverlaySystem = value;
}

Controller::Controller() {
    go();
}

Controller::~Controller() {
    if(nodeManager)
        delete nodeManager;

    if(oRoot)
        delete oRoot;
}

void Controller::go() {
    oRoot = new Ogre::Root();

    // alternatively, use ->restoreConfig() to load saved settings
    if(!oRoot->showConfigDialog()) {
        return;
    }

    // automatically create a window
    oRoot->initialise(true, Cycleshooter::RENDER_WINDOW_NAME);

    createSceneManager();
    createOverlaySystem();
    setupResources();

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    nodeManager = new NodeManager(this);
    nodeManager->setupRunnerMode();
}

void Controller::setupResources() {
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

void Controller::createSceneManager() {
    oSceneManager = oRoot->createSceneManager(Ogre::ST_GENERIC);
    oSceneManager->setDisplaySceneNodes(true);
}

void Controller::createOverlaySystem() {
    oOverlaySystem = new Ogre::OverlaySystem();
    oSceneManager->addRenderQueueListener(oOverlaySystem);
}

void Controller::setupRunnerMode() {
    context = CONTEXT_RUNNER;
    nodeManager->setupRunnerMode();
}

void Controller::setupShooterMode() {
    context = CONTEXT_SHOOTER;
    nodeManager->setupShooterMode();
}

void Controller::setupNoneMode() {
    context = CONTEXT_NONE;
    nodeManager->setupNoneMode();
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
