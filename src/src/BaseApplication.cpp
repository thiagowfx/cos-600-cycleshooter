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

void BaseApplication::go() {
    // randomness
    srand(time(NULL));

    // TODO: refine those peculiarities
    // TODO: change "cycleshooter" for the appropriate constant
    // TODO: set window icon
    window = new sf::Window(sf::VideoMode::getFullscreenModes()[0], "Cycleshooter", sf::Style::Fullscreen, sf::ContextSettings(32));

    mController = new Controller();

    // hud before the scene...[?]
    setupHUD();

    mController->setupDebugOn();

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

}
