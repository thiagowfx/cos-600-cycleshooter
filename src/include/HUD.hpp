#ifndef _HUD_HPP_
#define _HUD_HPP_

#include <Ogre.h>
#include <SdkTrays.h>

#include "Controller.hpp"

namespace Cycleshooter {
class Controller;

/**
 * @brief The HUD class
 * Companion Documentation: http://www.ogre3d.org/tikiwiki/SdkTrays
 */
class HUD {
    Controller* const controller = NULL;
    OgreBites::InputContext* const inputContext = NULL;

    /**
     * The tray manager. Used to create, update and delete widgets.
     * We should not create our widgets directly!
     */
    OgreBites::SdkTrayManager* trayManager = NULL;

    // go
    void go();
    void createTrayManager();
    void createTrayWidgets();

public:
    HUD(Controller* controller, OgreBites::InputContext* inputContext);
    virtual ~HUD();

    /**
     * Update the HUD elements.
     */
    void update(const Ogre::FrameEvent& evt);

    // setups
    void setupRunnerMode();
    void setupShooterMode();

    // debug
    void setupDebugOn();
    void setupDebugOff();
};
}

#endif
