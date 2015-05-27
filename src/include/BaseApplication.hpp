#ifndef _BASEAPPLICATION_HPP_
#define _BASEAPPLICATION_HPP_

#include <Ogre.h>
#include <SFML/Window.hpp>

#include "Controller.hpp"
#include "InputManager.hpp"

namespace Cycleshooter {
class Controller;

class BaseApplication : public Ogre::FrameListener {
public:
    BaseApplication();
    virtual ~BaseApplication();

private:
    void go();
    void setupFrameAndWindowListeners();
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    Cycleshooter::Controller* mController = NULL;
    sf::Window* window;
};

}

#endif
