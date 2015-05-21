#ifndef _BASEAPPLICATION_HPP_
#define _BASEAPPLICATION_HPP_

#include <Ogre.h>
#include <OIS.h>
#include <cstdlib>

#include "Controller.hpp"
#include "InputManager.hpp"
#include "HUD.hpp"

namespace Cycleshooter {
class Controller;

class BaseApplication : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener {
public:
    BaseApplication();
    virtual ~BaseApplication();

private:
    void go();
    void createFrameListener();
    void createScene();

    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);

    void cyclePolygonFilteringModeAction();
    void cyclePolygonRenderingModeAction();

    void setupKeyboardRunnerMapping();

    void gameMainLoop();

    Cycleshooter::Controller* mController = NULL;
    Cycleshooter::HUD* mHud = NULL;
    Cycleshooter::InputManager inputManager;

    bool mShutDown = false;

    OgreBites::InputContext mInputContext;
    OIS::InputManager* mInputManager = NULL;
};

}

#endif
