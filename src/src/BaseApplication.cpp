#include "BaseApplication.hpp"

namespace Cycleshooter {

BaseApplication::BaseApplication()
{
    go();
}

BaseApplication::~BaseApplication() {
    Ogre::WindowEventUtilities::removeWindowEventListener(mController->getWindow(), this);
    windowClosed(mController->getWindow());

    if(mHud)
        delete mHud;

    if(mController)
        delete mController;
}

void BaseApplication::initializeOIS() {
    Ogre::LogManager::getSingletonPtr()->logMessage("--> BaseApplication: Initializing OIS <--");

    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mController->getWindow()->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem(pl);

    // KeyboardListener
    mInputContext.mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
    mInputContext.mKeyboard->setEventCallback(this);

    // MouseListener
    mInputContext.mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
    mInputContext.mMouse->setEventCallback(this);

    // set initial mouse clipping size
    windowResized(mController->getWindow());
}

void BaseApplication::setupFrameAndWindowListeners() {
    Ogre::LogManager::getSingletonPtr()->logMessage("--> BaseApplication: Setting up Frame Listener <--");

    // WindowListener
    Ogre::WindowEventUtilities::addWindowEventListener(mController->getWindow(), this);

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

    mController = new Controller();

    initializeOIS();
    setupHUD();
    createScene();

    mController->setupDebugOn();

    setupKeyboardRunnerMapping();
    setupFrameAndWindowListeners();

    // alternatively, Ogre's own loop: Ogre::Root::startRendering() + listeners
    mController->gameMainLoop();
}

bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    if(mController->getWindow()->isClosed())
        mController->shutdownNow();

    if(mController->getShutdown()) {

        // sync with other threads for a clean shutdown
        mController->getPolarUpdater()->wait();

        return false;
    }

    // capture/update each device
    mInputContext.capture();

    mHud->update(evt);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        InputManager::instance().executeAction(sf::Keyboard::Escape, mController->getContext());
    }

    // if (!mHUD->getTrayManager()->isDialogVisible()) {
        // If dialog isn't up, then update the camera
        // mCameraMan->frameRenderingQueued(evt);
        // if (mHUD->isDebugPanelVisible()) {
        //     mHUD->updateDebugPanel_CameraElements(mController->getNodeManager()->getMainCamera());
        // }
    // }

    return true;
}

bool BaseApplication::keyPressed(const OIS::KeyEvent &arg) {
    // InputManager::executeAction(key, mController->getContext());
    // mCameraMan->injectKeyDown(arg);
    return true;
}

bool BaseApplication::keyReleased(const OIS::KeyEvent &arg) {
    // mCameraMan->injectKeyUp(arg);
    return true;
}

bool BaseApplication::mouseMoved(const OIS::MouseEvent &arg) {
    // mCameraMan->injectMouseMove(arg);
    return true;
}

bool BaseApplication::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    // mCameraMan->injectMouseDown(arg, id);
    return true;
}

bool BaseApplication::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    // mCameraMan->injectMouseUp(arg, id);
    return true;
}

void BaseApplication::windowResized(Ogre::RenderWindow* rw) {
    Ogre::LogManager::getSingleton().logMessage("--> BaseApplication: Window Resized <--");

    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mInputContext.mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

// unattach OIS before window shutdown (very important under Linux)
void BaseApplication::windowClosed(Ogre::RenderWindow* rw) {
    Ogre::LogManager::getSingleton().logMessage("--> BaseApplication: Window Closed <--");

    // only close for window that created OIS (the main window)
    if(rw == mController->getWindow()) {
        if(mInputManager) {
            mInputManager->destroyInputObject(mInputContext.mMouse);
            mInputManager->destroyInputObject(mInputContext.mKeyboard);
            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = NULL;
        }
    }
}

void BaseApplication::cyclePolygonRenderingMode() {
    Ogre::String newVal;
    Ogre::PolygonMode pm;

    switch (mController->getNodeManager()->getMainCamera()->getPolygonMode()) {
    case Ogre::PM_SOLID:
        newVal = "Wireframe";
        pm = Ogre::PM_WIREFRAME;
        break;
    case Ogre::PM_WIREFRAME:
        newVal = "Points";
        pm = Ogre::PM_POINTS;
        break;
    default:
        newVal = "Solid";
        pm = Ogre::PM_SOLID;
    }

    mController->getNodeManager()->getMainCamera()->setPolygonMode(pm);
}

void BaseApplication::setupKeyboardRunnerMapping() {
//    inputManager.addOrUpdateBinding(OIS::KC_R, [&]{
//        cyclePolygonRenderingMode();
//    });

//    // reload all textures
//    inputManager.addOrUpdateBinding(OIS::KC_F5, [&]{
//        Ogre::TextureManager::getSingleton().reloadAll();
//    });

//    // take a screenshot
//    inputManager.addOrUpdateBinding(OIS::KC_SYSRQ, [&]{
//        mController->getWindow()->writeContentsToTimestampedFile("screenshot", ".jpg");
//    });

    // quit from the application
    InputManager::instance().addKey(sf::Keyboard::Escape, [&]{
        mController->shutdownNow();
    });

//    inputManager.addOrUpdateBinding({OIS::KC_W,
//                                     OIS::KC_UP}, [&]{
//        mController->getNodeManager()->getParentPlayerSceneNode()->translate(Ogre::Vector3(0.0, 0.0, -10.0), Ogre::SceneNode::TS_LOCAL);
//    });

//    inputManager.addOrUpdateBinding({OIS::KC_S,
//                                     OIS::KC_DOWN}, [&]{
//        mController->getNodeManager()->getParentPlayerSceneNode()->translate(Ogre::Vector3(0.0, 0.0, +10.0), Ogre::SceneNode::TS_LOCAL);
//    });

//    inputManager.addOrUpdateBinding({OIS::KC_A,
//                                     OIS::KC_LEFT}, [&]{
//        mController->getNodeManager()->getParentPlayerSceneNode()->yaw(Ogre::Degree(+10.0));
//    });

//    inputManager.addOrUpdateBinding({OIS::KC_D,
//                                     OIS::KC_RIGHT}, [&]{
//        mController->getNodeManager()->getParentPlayerSceneNode()->yaw(Ogre::Degree(-10.0));
//    });

//    inputManager.addOrUpdateBinding(OIS::KC_1, [&]{
//        mController->toggleMode();
//    });

//    inputManager.addOrUpdateBinding(OIS::KC_2, [&]{
//        mController->toggleDebug();
//    });
}

}
