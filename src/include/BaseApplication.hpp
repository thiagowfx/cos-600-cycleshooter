#ifndef _BASEAPPLICATION_HPP_
#define _BASEAPPLICATION_HPP_

#include <Ogre.h>
#include <SFML/Window.hpp>
#include <cstdlib>

#include "Controller.hpp"
#include "InputManager.hpp"
#include "HUD.hpp"

namespace Cycleshooter {
class Controller;

class BaseApplication : public Ogre::FrameListener {
public:
    BaseApplication();
    virtual ~BaseApplication();

private:
    void go();
    void setupFrameAndWindowListeners();
    void setupHUD();
    void createScene();
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    void setupMappings();

    Cycleshooter::Controller* mController = NULL;
    Cycleshooter::HUD* mHud = NULL;
    sf::Window* window;
};

}

#endif
