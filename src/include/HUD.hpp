#ifndef _HUD_HPP_
#define _HUD_HPP_

#include <Ogre.h>
#include <SdkTrays.h>

#include "Controller.hpp"

namespace Cycleshooter {
class Controller;

class HUD {
    Controller* controller = NULL;

    OgreBites::SdkTrayManager* trayManager = NULL;

    void go();
    void createTrayManager();

public:
    HUD(Controller* controller);
    virtual ~HUD();

    void setupRunnerMode();
    void setupShooterMode();
    void setupNoneMode();

    // getters and setters
    Controller *getController() const;
    void setController(Controller *value);
    OgreBites::SdkTrayManager *getTrayManager() const;
    void setTrayManager(OgreBites::SdkTrayManager *value);
};
}

#endif
