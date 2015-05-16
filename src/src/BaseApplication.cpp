#include "BaseApplication.hpp"

namespace Cycleshooter {

BaseApplication::BaseApplication()
    : mRoot(0),
    mWindow(0),
    mContextManager(0),
    mResources(0),
 // mCameraMan(0),
    mHUD(0),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0) {
}

BaseApplication::~BaseApplication() {
    if (mHUD)
        delete mHUD;
//    if (mCameraMan)
//        delete mCameraMan;
    if(mContextManager)
        delete mContextManager;

    // Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    if(mResources)
        delete mResources;
    delete mRoot;
}

//void BaseApplication::createCamera() {
//    mFrontSceneNode->setPosition(0.0, 0.0, 100.0);
//    mFrontCamera->lookAt(Ogre::Vector3(0.0, 0.0, -1.0));

//    // Create a default camera controller
//    // mCameraMan = new OgreBites::SdkCameraMan(mFrontCamera);
//}

void BaseApplication::createFrameListener() {
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem(pl);

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    // Set initial mouse clipping size
    windowResized(mWindow);

    // Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mInputContext.mKeyboard = mKeyboard;
    mInputContext.mMouse = mMouse;
    mHUD = new HUD("InterfaceName", mWindow, mInputContext, this);

    mRoot->addFrameListener(this);
}

void BaseApplication::createScene() {
    mContextManager->getSceneManager()->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    Ogre::Entity* ogreEntity = mContextManager->getSceneManager()->createEntity("ogrehead.mesh");
    Ogre::Entity* ogreEntity2 = mContextManager->getSceneManager()->createEntity("ogrehead.mesh");
    Ogre::Entity* ogreEntity3 = mContextManager->getSceneManager()->createEntity("ogrehead.mesh");
    Ogre::SceneNode* ogreNode = mContextManager->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    Ogre::SceneNode* ogreNode2 = mContextManager->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    Ogre::SceneNode* ogreNode3 = mContextManager->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    ogreNode2->translate(0.0, 0.0, 400.0);
    ogreNode3->translate(0.0, 0.0, -400.0);
    ogreNode->attachObject(ogreEntity);
    ogreNode2->attachObject(ogreEntity2);
    ogreNode3->attachObject(ogreEntity3);
    Ogre::Light* light =  mContextManager->getSceneManager()->createLight("MainLight");
    light->setPosition(20.0, 80.0, 50.0);
}

void BaseApplication::go() {
    mResources = new Cycleshooter::Resources();
    mRoot = new Ogre::Root(mResources->getPluginConfig());
    mResources->setupResources();

    // Show the configuration dialog and initialise the system.
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg.
    if(!mRoot->showConfigDialog()) {
        return;
    }

    // If returned true, user clicked OK so initialise.
    // Here we choose to let the system create a default rendering window by passing 'true'.
    mWindow = mRoot->initialise(true, Cycleshooter::RENDER_WINDOW_NAME);

    mContextManager = new ContextManager(mRoot);
    mContextManager->setupRunnerMode();

    //mContextManager->getCameraParentSceneNode()->setPosition(0.0, 0.0, 200.0);
    //mContextManager->getMainCamera()->lookAt(0.0, 0.0, -1.0);

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    createScene();

    createFrameListener();

    setupKeyboardRunnerMapping();

    // OGRE's own loop
    // mRoot->startRendering();
    // Our own loop
    gameMainLoop();
}

bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    // Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    mHUD->update(evt);

    if (!mHUD->getTrayManager()->isDialogVisible()) {
        // If dialog isn't up, then update the camera
        // mCameraMan->frameRenderingQueued(evt);
        if (mHUD->isDebugPanelVisible()) {
            mHUD->updateDebugPanel_CameraElements(mContextManager->getMainCamera());
        }
    }

    return true;
}

bool BaseApplication::keyPressed( const OIS::KeyEvent &arg) {
    if (mHUD->getTrayManager()->isDialogVisible())
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
    if (mHUD->getTrayManager()->injectMouseMove(arg))
        return true;
    // mCameraMan->injectMouseMove(arg);
    return true;
}

bool BaseApplication::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    if (mHUD->getTrayManager()->injectMouseDown(arg, id))
        return true;
    // mCameraMan->injectMouseDown(arg, id);
    return true;
}

bool BaseApplication::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    if (mHUD->getTrayManager()->injectMouseUp(arg, id))
        return true;
    // mCameraMan->injectMouseUp(arg, id);
    return true;
}

void BaseApplication::windowResized(Ogre::RenderWindow* rw) {
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

// Unattach OIS before window shutdown (very important under Linux)
void BaseApplication::windowClosed(Ogre::RenderWindow* rw) {
    // Only close for window that created OIS (the main window in these demos)
    if(rw == mWindow)
    {
        if(mInputManager)
        {
            mInputManager->destroyInputObject(mMouse);
            mInputManager->destroyInputObject(mKeyboard);

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

void BaseApplication::cyclePolygonFilteringModeAction() {
    Ogre::String newVal;
    Ogre::TextureFilterOptions tfo;
    unsigned int aniso;
    const std::string filter = mHUD->getDebugPanel_PolygonFilteringElement();

    if(filter == "Bilinear") {
        newVal = "Trilinear";
        tfo = Ogre::TFO_TRILINEAR;
        aniso = 1;
    }
    else if (filter == "Trilinear") {
        newVal = "Anisotropic";
        tfo = Ogre::TFO_ANISOTROPIC;
        aniso = 8;
    }
    else if (filter == "Anisotropic") {
        newVal = "None";
        tfo = Ogre::TFO_NONE;
        aniso = 1;
    }
    else {
        newVal = "Bilinear";
        tfo = Ogre::TFO_BILINEAR;
        aniso = 1;
    }

    Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
    mHUD->setDebugPanel_PolygonFilteringElement(newVal);
}

void BaseApplication::cyclePolygonRenderingModeAction() {
    Ogre::String newVal;
    Ogre::PolygonMode pm;

    switch (mContextManager->getMainCamera()->getPolygonMode()) {
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

    mContextManager->getMainCamera()->setPolygonMode(pm);
    mHUD->setDebugPanel_PolygonRenderingElement(newVal);
}

void BaseApplication::setupKeyboardRunnerMapping() {
    inputManager.addOrUpdateBinding(OIS::KC_G,
                                    [&]() {
        mHUD->toggleDebugPanel();
        mHUD->toggleFPSPanel();
    });

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
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    });

    // quit from the application
    inputManager.addOrUpdateBinding(OIS::KC_ESCAPE, [&]{
        mShutDown = true;
    });

    inputManager.addOrUpdateBinding({OIS::KC_W,
                                     OIS::KC_UP}, [&]{
        mContextManager->getParentSceneNode()->translate(Ogre::Vector3(0.0, 0.0, -10.0), Ogre::SceneNode::TS_LOCAL);
    });

    inputManager.addOrUpdateBinding({OIS::KC_S,
                                     OIS::KC_DOWN}, [&]{
        mContextManager->getParentSceneNode()->translate(Ogre::Vector3(0.0, 0.0, +10.0), Ogre::SceneNode::TS_LOCAL);
    });

    inputManager.addOrUpdateBinding({OIS::KC_A,
                                     OIS::KC_LEFT}, [&]{
        mContextManager->getParentSceneNode()->yaw(Ogre::Degree(+10.0));
    });

    inputManager.addOrUpdateBinding({OIS::KC_D,
                                     OIS::KC_RIGHT}, [&]{
        mContextManager->getParentSceneNode()->yaw(Ogre::Degree(-10.0));
    });

    inputManager.addOrUpdateBinding(OIS::KC_SPACE, [&]{
        mContextManager->toggleMode();
    });
}

void BaseApplication::gameMainLoop() {
    while(!mShutDown) {
        mRoot->renderOneFrame();
    }
}

}
