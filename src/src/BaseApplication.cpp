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

void BaseApplication::createFrameListener() {
    Ogre::LogManager::getSingletonPtr()->logMessage("--> Initializing OIS <--");

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

    // Set initial mouse clipping size
    windowResized(mController->getWindow());

    // FrameListener
    mController->getRoot()->addFrameListener(this);

    mHud = new HUD(mController, &mInputContext);
    mHud->setupRunnerMode();
    mController->setHud(mHud);
}

void BaseApplication::createScene() {
    Ogre::LogManager::getSingleton().logMessage("--> Creating Scene <--");

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
    srand(time(NULL));

    mController = new Controller();

    // WindowListener
    Ogre::WindowEventUtilities::addWindowEventListener(mController->getWindow(), this);

    createFrameListener();

    createScene();

    setupKeyboardRunnerMapping();

    mController->setupDebugModeOn();

    // OGRE's own loop
    // mRoot->startRendering();
    // Our own loop
    gameMainLoop();
}

bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    if(mController->getWindow()->isClosed() || mController->getShutdown()) {
        mController->getPolarUpdater()->wait();
        return false;
    }

    // capture/update each device
    mInputContext.capture();

    mHud->update(evt);

    // if (!mHUD->getTrayManager()->isDialogVisible()) {
        // If dialog isn't up, then update the camera
        // mCameraMan->frameRenderingQueued(evt);
        // if (mHUD->isDebugPanelVisible()) {
        //     mHUD->updateDebugPanel_CameraElements(mController->getNodeManager()->getMainCamera());
        // }
    // }

    return true;
}

bool BaseApplication::keyPressed( const OIS::KeyEvent &arg) {
    if(mController->getHud()->getTrayManager()->isDialogVisible())
        return true;   // don't process any more keys if dialog is up

    inputManager.executeAction(arg.key);

    // mCameraMan->injectKeyDown(arg);
    return true;
}

bool BaseApplication::keyReleased(const OIS::KeyEvent &arg) {
    // mCameraMan->injectKeyUp(arg);
    return true;
}

bool BaseApplication::mouseMoved(const OIS::MouseEvent &arg) {
    if(mController->getHud()->getTrayManager()->injectMouseMove(arg))
        return true;
    // mCameraMan->injectMouseMove(arg);
    return true;
}

bool BaseApplication::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    if(mController->getHud()->getTrayManager()->injectMouseDown(arg, id))
        return true;
    // mCameraMan->injectMouseDown(arg, id);
    return true;
}

bool BaseApplication::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    if (mController->getHud()->getTrayManager()->injectMouseUp(arg, id))
        return true;
    // mCameraMan->injectMouseUp(arg, id);
    return true;
}

void BaseApplication::windowResized(Ogre::RenderWindow* rw) {
    Ogre::LogManager::getSingleton().logMessage("--> Window Resized <--");

    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mInputContext.mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

// Unattach OIS before window shutdown (very important under Linux)
void BaseApplication::windowClosed(Ogre::RenderWindow* rw) {
    Ogre::LogManager::getSingleton().logMessage("--> Window Closed <--");

    // Only close for window that created OIS (the main window in these demos)
    if(rw == mController->getWindow()) {
        if(mInputManager) {
            mInputManager->destroyInputObject(mInputContext.mMouse);
            mInputManager->destroyInputObject(mInputContext.mKeyboard);
            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = NULL;
        }
    }
}

void BaseApplication::cyclePolygonFilteringModeAction() {
//    Ogre::String newVal;
//    Ogre::TextureFilterOptions tfo;
//    unsigned int aniso;
    // const std::string filter = mHUD->getDebugPanel_PolygonFilteringElement();

//    if(filter == "Bilinear") {
//        newVal = "Trilinear";
//        tfo = Ogre::TFO_TRILINEAR;
//        aniso = 1;
//    }
//    else if (filter == "Trilinear") {
//        newVal = "Anisotropic";
//        tfo = Ogre::TFO_ANISOTROPIC;
//        aniso = 8;
//    }
//    else if (filter == "Anisotropic") {
//        newVal = "None";
//        tfo = Ogre::TFO_NONE;
//        aniso = 1;
//    }
//    else {
//        newVal = "Bilinear";
//        tfo = Ogre::TFO_BILINEAR;
//        aniso = 1;
//    }

//    Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
//    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
    // mHUD->setDebugPanel_PolygonFilteringElement(newVal);
}

void BaseApplication::cyclePolygonRenderingModeAction() {
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
    // mHUD->setDebugPanel_PolygonRenderingElement(newVal);
}

void BaseApplication::setupKeyboardRunnerMapping() {
    inputManager.addOrUpdateBinding(OIS::KC_R, [&]{
        cyclePolygonRenderingModeAction();
    });

    inputManager.addOrUpdateBinding(OIS::KC_T, [&]{
        cyclePolygonFilteringModeAction();
    });

    // reload all textures
    inputManager.addOrUpdateBinding(OIS::KC_F5, [&]{
        Ogre::TextureManager::getSingleton().reloadAll();
    });

    // take a screenshot
    inputManager.addOrUpdateBinding(OIS::KC_SYSRQ, [&]{
        mController->getWindow()->writeContentsToTimestampedFile("screenshot", ".jpg");
    });

    // quit from the application
    inputManager.addOrUpdateBinding(OIS::KC_ESCAPE, [&]{
        mController->shutdownNow();
    });

    inputManager.addOrUpdateBinding({OIS::KC_W,
                                     OIS::KC_UP}, [&]{
        mController->getNodeManager()->getParentPlayerSceneNode()->translate(Ogre::Vector3(0.0, 0.0, -10.0), Ogre::SceneNode::TS_LOCAL);
    });

    inputManager.addOrUpdateBinding({OIS::KC_S,
                                     OIS::KC_DOWN}, [&]{
        mController->getNodeManager()->getParentPlayerSceneNode()->translate(Ogre::Vector3(0.0, 0.0, +10.0), Ogre::SceneNode::TS_LOCAL);
    });

    inputManager.addOrUpdateBinding({OIS::KC_A,
                                     OIS::KC_LEFT}, [&]{
        mController->getNodeManager()->getParentPlayerSceneNode()->yaw(Ogre::Degree(+10.0));
    });

    inputManager.addOrUpdateBinding({OIS::KC_D,
                                     OIS::KC_RIGHT}, [&]{
        mController->getNodeManager()->getParentPlayerSceneNode()->yaw(Ogre::Degree(-10.0));
    });

    inputManager.addOrUpdateBinding(OIS::KC_SPACE, [&]{
        mController->toggleMode();
    });

    inputManager.addOrUpdateBinding(OIS::KC_1, [&]{
        mController->setupRunnerMode();
    });

    inputManager.addOrUpdateBinding(OIS::KC_2, [&]{
        mController->setupShooterMode();
    });

    inputManager.addOrUpdateBinding(OIS::KC_3, [&]{
        mController->toggleDebugMode();
    });
}

void BaseApplication::gameMainLoop() {
    Ogre::LogManager::getSingleton().logMessage("--> Game Main Loop <--");

    while(!mController->getShutdown()) {
        mController->getRoot()->renderOneFrame();
    }
}

}
