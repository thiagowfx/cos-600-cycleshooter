#include "BaseApplication.hpp"

namespace Cycleshooter {

BaseApplication::BaseApplication()
{
    go();
}

BaseApplication::~BaseApplication() {
    if(window)
        delete window;

    if(mHud)
        delete mHud;

    if(mController)
        delete mController;
}

void BaseApplication::setupFrameAndWindowListeners() {
    Ogre::LogManager::getSingletonPtr()->logMessage("--> BaseApplication: Setting up Frame Listener <--");

    // FrameListener
    mController->getRoot()->addFrameListener(this);
}

void BaseApplication::setupHUD() {
    Ogre::LogManager::getSingletonPtr()->logMessage("--> BaseApplication: Setting up HUD <--");

    mHud = new HUD(mController);
    mHud->setHelpPanel({"1","2"},{"ToggleMode","ToggleDebug"});
    mHud->setupRunnerMode();
    mController->setHud(mHud);
}

void BaseApplication::createScene() {
    Ogre::LogManager::getSingleton().logMessage("--> BaseApplication: Creating Scene <--");

    mController->getSceneManager()->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    Ogre::Entity* ogreEntity = mController->getSceneManager()->createEntity("ogrehead.mesh");
    Ogre::Entity* ogreEntity2 = mController->getSceneManager()->createEntity("ogrehead.mesh");
    Ogre::Entity* ogreEntity3 = mController->getSceneManager()->createEntity("ogrehead.mesh");
    Ogre::SceneNode* ogreNode = mController->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    Ogre::SceneNode* ogreNode2 = mController->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    Ogre::SceneNode* ogreNode3 = mController->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    ogreNode2->translate(0.0, 0.0, 400.0);
    ogreNode3->translate(0.0, 0.0, -400.0);
    ogreNode->attachObject(ogreEntity);
    ogreNode2->attachObject(ogreEntity2);
    ogreNode3->attachObject(ogreEntity3);
    Ogre::Light* light =  mController->getSceneManager()->createLight("MainLight");
    light->setPosition(20.0, 80.0, 50.0);
}

void BaseApplication::go() {
    // randomness
    srand(time(NULL));

    // TODO: refine those peculiarities
    // TODO: change "cycleshooter" for the appropriate constant
    // TODO: set window icon
    window = new sf::Window(sf::VideoMode::getFullscreenModes()[0], "Cycleshooter", sf::Style::Fullscreen, sf::ContextSettings(32));

    mController = new Controller();

    setupHUD();
    createScene();

    mController->setupDebugOn();

    setupMappings();
    setupFrameAndWindowListeners();

    // alternatively, Ogre's own loop: Ogre::Root::startRendering() + listeners
    mController->gameMainLoop();
}

bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    Ogre::WindowEventUtilities::messagePump();

    if(mController->getWindow()->isClosed())
        mController->shutdownNow();

    if(mController->getShutdown()) {

        // sync with other threads for a clean shutdown
        mController->getPolarUpdater()->wait();

        return false;
    }

    // TODO: update logic[manager] here somewhere
    // TODO: add/use clock for unbuf
    // TODO key->mapping massive rename
    // TODO: merge with Podolan's branch

    mHud->update(evt);

    // process unbuffered keys
    InputManager::instance().executeActionUnbuf(mController->getContext());

    // process events (in particular, buffered keys)
    sf::Event event;

    // TODO: check if there is the possibility of an infinite event loop
    // while there are pending events...
    while (window->pollEvent(event)) {

        // check the type of the event...
        switch (event.type) {

            // window closed
            case sf::Event::Closed:
                window->close();
                mController->shutdownNow();
                break;

            // TODO: resize event (adjust viewport)

            // key pressed
            case sf::Event::KeyPressed:
                InputManager::instance().executeAction(event.key.code, mController->getContext());
                break;

            // TODO: add joystick events
            // TODO: add mouse events

            // don't process other types of events
            default:
                break;
        }
    }

    return true;
}

void BaseApplication::setupMappings() {
    // refresh (reload) all textures
    InputManager::instance().addKey(sf::Keyboard::F5, [&] {
       Ogre::TextureManager::getSingleton().reloadAll();
    });

    // take a screenshot
    InputManager::instance().addKey(sf::Keyboard::Pause, [&] {
        mController->getWindow()->writeContentsToTimestampedFile("screenshot", ".jpg");
    });

    // quit from the application
    InputManager::instance().addKeyUnbuf(sf::Keyboard::Escape, [&]{
        mController->shutdownNow();
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::W,
                                           sf::Keyboard::Up}, CONTEXT_RUNNER, [&]{
        mController->getNodeManager()->getParentPlayerSceneNode()->translate(Ogre::Vector3(0.0, 0.0, -10.0), Ogre::SceneNode::TS_LOCAL);
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::S,
                                           sf::Keyboard::Down}, CONTEXT_RUNNER, [&]{
        mController->getNodeManager()->getParentPlayerSceneNode()->translate(Ogre::Vector3(0.0, 0.0, +10.0), Ogre::SceneNode::TS_LOCAL);
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::A,
                                           sf::Keyboard::Left}, CONTEXT_RUNNER, [&]{
        mController->getNodeManager()->getParentPlayerSceneNode()->yaw(Ogre::Degree(+10.0));
    });

    InputManager::instance().addKeysUnbuf({sf::Keyboard::D,
                                           sf::Keyboard::Right}, CONTEXT_RUNNER, [&]{
        mController->getNodeManager()->getParentPlayerSceneNode()->yaw(Ogre::Degree(-10.0));
    });

    InputManager::instance().addAxisUnbuf(sf::Joystick::X, CONTEXT_RUNNER, [&](float f){
        mController->getNodeManager()->getParentPlayerSceneNode()->yaw(Ogre::Degree(-10 * f / 100.0));
    });

    InputManager::instance().addAxisUnbuf(sf::Joystick::Y, CONTEXT_RUNNER, [&](float f){
        mController->getNodeManager()->getParentPlayerSceneNode()->translate(Ogre::Vector3(0.0, 0.0, 10.0 * f / 100.0), Ogre::SceneNode::TS_LOCAL);
    });


    InputManager::instance().addKey(sf::Keyboard::Num1, [&]{
        mController->toggleMode();
    });

    InputManager::instance().addKey(sf::Keyboard::Num2, [&]{
        mController->toggleDebug();
    });
}

}
