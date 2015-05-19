#ifndef _HUD_HPP_
#define _HUD_HPP_

#include <Ogre.h>
#include <SdkTrays.h>

#include "Controller.hpp"

namespace Cycleshooter {
class Controller;

class HUD {
    Controller* const controller = NULL;
    OgreBites::InputContext* const inputContext = NULL;

    OgreBites::SdkTrayManager* trayManager = NULL;

    OgreBites::Widget* contextWidget = NULL;

    // go
    void go();
    void createTrayManager();
    void createTrayWidgets();

public:
    HUD(Controller* controller, OgreBites::InputContext* inputContext);
    virtual ~HUD();

    void update(const Ogre::FrameEvent& evt);

    // setups
    void setupRunnerMode();
    void setupShooterMode();

    // debug
    void setupDebugOn();
    void setupDebugOff();

    // getters and setters
    Controller *getController() const;
    OgreBites::SdkTrayManager *getTrayManager() const;
};
}

#endif
