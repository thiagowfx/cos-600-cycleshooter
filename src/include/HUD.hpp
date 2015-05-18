#ifndef _HUD_HPP_
#define _HUD_HPP_

#include <Ogre.h>
#include <SdkTrays.h>

#include "BaseApplication.hpp"
#include "Controller.hpp"

namespace Cycleshooter {
class BaseApplication;
class Controller;

class HUD {
    Controller* const controller = NULL;
    OgreBites::InputContext* const inputContext = NULL;
    Cycleshooter::BaseApplication* const baseApplication = NULL;

    OgreBites::SdkTrayManager* trayManager = NULL;

    void go();
    void createTrayManager();

public:
    HUD(Controller* controller, OgreBites::InputContext* inputContext, Cycleshooter::BaseApplication* baseApplication);
    virtual ~HUD();

    void setupRunnerMode();
    void setupShooterMode();
    void setupNoneMode();

    // getters and setters
    Controller *getController() const;
    OgreBites::SdkTrayManager *getTrayManager() const;
};
}

#endif
