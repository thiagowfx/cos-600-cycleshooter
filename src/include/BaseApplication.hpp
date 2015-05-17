#ifndef _BASEAPPLICATION_HPP_
#define _BASEAPPLICATION_HPP_

#include <Ogre.h>
#include <OIS.h>
#include <SdkTrays.h>

#include "Controller.hpp"
#include "HUD.hpp"
#include "InputManager.hpp"

namespace Cycleshooter {

class BaseApplication : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
    BaseApplication();
    virtual ~BaseApplication();

    void go();

protected:
    void createFrameListener();
    void createScene();

    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    bool keyPressed(const OIS::KeyEvent &arg);
    bool keyReleased(const OIS::KeyEvent &arg);
    bool mouseMoved(const OIS::MouseEvent &arg);
    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    void windowResized(Ogre::RenderWindow* rw);
    void windowClosed(Ogre::RenderWindow* rw);

    void cyclePolygonFilteringModeAction();
    void cyclePolygonRenderingModeAction();

    void setupKeyboardRunnerMapping();

    void gameMainLoop();

    Cycleshooter::Controller* mController = NULL;
    Cycleshooter::HUD* mHUD = NULL;
    Cycleshooter::InputManager inputManager;

    bool mCursorWasVisible = false;
    bool mShutDown = false;

    OgreBites::InputContext mInputContext;
    OIS::InputManager* mInputManager = NULL;
    OIS::Mouse* mMouse = NULL;
    OIS::Keyboard* mKeyboard = NULL;
};

}

#endif
