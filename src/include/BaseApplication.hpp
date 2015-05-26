#ifndef _BASEAPPLICATION_HPP_
#define _BASEAPPLICATION_HPP_

#include <Ogre.h>
#include <cstdlib>

#include "Controller.hpp"
#include "InputManager.hpp"
#include "HUD.hpp"

namespace Cycleshooter {
class Controller;

class BaseApplication : public Ogre::FrameListener, public Ogre::WindowEventListener {
public:
    BaseApplication();
    virtual ~BaseApplication();

private:
    void go();
    void setupFrameAndWindowListeners();
    void setupHUD();
    void createScene();
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    void cyclePolygonRenderingMode();

    void setupMappings();

    Cycleshooter::Controller* mController = NULL;
    Cycleshooter::HUD* mHud = NULL;
};

}

#endif
