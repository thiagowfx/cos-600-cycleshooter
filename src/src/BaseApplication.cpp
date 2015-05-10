#include "BaseApplication.hpp"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <macUtils.h>
#endif

namespace Cycleshooter {

BaseApplication::BaseApplication()
    : mRoot(0),
    mSceneMgr(0),
    mWindow(0),
    mContextManager(0),
 // mCameraMan(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mHUD(0),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0),
    mOverlaySystem(0) {
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    m_ResourcePath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
    m_ResourcePath = "";
#endif
}

BaseApplication::~BaseApplication() {
    if (mHUD)
        delete mHUD;
//    if (mCameraMan)
//        delete mCameraMan;
    if(mContextManager)
        delete mContextManager;
    if (mOverlaySystem)
        delete mOverlaySystem;

    // Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

void BaseApplication::chooseSceneManager() {
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

    // Initialize the OverlaySystem (changed for Ogre 1.9)
    mOverlaySystem = new Ogre::OverlaySystem();
    mSceneMgr->addRenderQueueListener(mOverlaySystem);
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
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    Ogre::Entity* ogreEntity = mSceneMgr->createEntity("ogrehead.mesh");
    Ogre::Entity* ogreEntity2 = mSceneMgr->createEntity("ogrehead.mesh");
    Ogre::Entity* ogreEntity3 = mSceneMgr->createEntity("ogrehead.mesh");
    Ogre::SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    Ogre::SceneNode* ogreNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    Ogre::SceneNode* ogreNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    ogreNode2->translate(0.0, 0.0, 400.0);
    ogreNode3->translate(0.0, 0.0, -400.0);
    ogreNode->attachObject(ogreEntity);
    ogreNode2->attachObject(ogreEntity2);
    ogreNode3->attachObject(ogreEntity3);
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(20.0, 80.0, 50.0);
}



void BaseApplication::destroyScene() {
}

void BaseApplication::setupResources() {
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            // OS X does not set the working directory relative to the app.
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location.
            if (!Ogre::StringUtil::startsWith(archName, "/", false)) // only adjust relative directories
                archName = Ogre::String(Ogre::macBundlePath() + "/" + archName);
#endif

            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}

void BaseApplication::go() {
#ifdef _DEBUG
#ifndef OGRE_STATIC_LIB
    mResourcesCfg = m_ResourcePath + "resources_d.cfg";
    mPluginsCfg = m_ResourcePath + "plugins_d.cfg";
#else
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#endif
#else
#ifndef OGRE_STATIC_LIB
    mResourcesCfg = m_ResourcePath + "resources.cfg";
    mPluginsCfg = m_ResourcePath + "plugins.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif
#endif

    // plugins.cfg
    mRoot = new Ogre::Root(mPluginsCfg);

    // resources.cfg
    setupResources();

    // Show the configuration dialog and initialise the system.
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg.
    if(!mRoot->showConfigDialog()) {
        return;
    }

    // If returned true, user clicked OK so initialise.
    // Here we choose to let the system create a default rendering window by passing 'true'.
    mWindow = mRoot->initialise(true, Cycleshooter::RENDER_WINDOW_NAME);

    chooseSceneManager();

    mContextManager = new ContextManager(mSceneMgr, mWindow);
    mContextManager->setupRunnerMode();

    //mContextManager->getCameraParentSceneNode()->setPosition(0.0, 0.0, 200.0);
    //mContextManager->getMainCamera()->lookAt(0.0, 0.0, -1.0);

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    createScene();

    createFrameListener();

    // OGRE's own loop
    // mRoot->startRendering();
    // Our own loop
    gameMainLoop();

    // Clean up
    destroyScene();
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

void BaseApplication::toggleMode() {
    mContextManager->toggleMode();
}

bool BaseApplication::keyPressed( const OIS::KeyEvent &arg ) {
    if (mHUD->getTrayManager()->isDialogVisible())
        return true;   // don't process any more keys if dialog is up

    switch(arg.key) {
    case OIS::KC_G:
        mHUD->toggleDebugPanel();
        mHUD->toggleFPSPanel();
        break;
    case OIS::KC_R:
        cyclePolygonRenderingModeAction();
        break;
    case OIS::KC_T:
        cyclePolygonFilteringModeAction();
        break;
    case OIS::KC_F5:
        // refresh all textures
        Ogre::TextureManager::getSingleton().reloadAll();
        break;
    case OIS::KC_SYSRQ:
        // take a screenshot
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
        break;
    case OIS::KC_ESCAPE:
        mShutDown = true;
        break;

    case OIS::KC_W:
        mContextManager->getCameraParentSceneNode()->translate(Ogre::Vector3(0.0, 0.0, -10.0), Ogre::SceneNode::TS_LOCAL);
        break;
    case OIS::KC_S:
        mContextManager->getCameraParentSceneNode()->translate(Ogre::Vector3(0.0, 0.0, +10.0), Ogre::SceneNode::TS_LOCAL);
        break;
    case OIS::KC_A:
        mContextManager->getCameraParentSceneNode()->yaw(Ogre::Degree(10.0));
        break;
    case OIS::KC_D:
        mContextManager->getCameraParentSceneNode()->yaw(Ogre::Degree(-10.0));
        break;
    case OIS::KC_SPACE:
        toggleMode();
        break;

    }

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

void BaseApplication::gameMainLoop() {
    while(!mShutDown) {
        mRoot->renderOneFrame();
    }
}

}
