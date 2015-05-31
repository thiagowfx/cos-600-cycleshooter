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

    /**
     * The tray manager. Used to create, update and delete widgets.
     * We should not create our widgets directly!
     */
    OgreBites::SdkTrayManager* trayManager = NULL;

    // go
    void go();
    void createTrayManager();
    void createTrayWidgets();

    // customizable settings
    const OgreBites::TrayLocation CONTEXT_TL = OgreBites::TL_TOPLEFT;
    const OgreBites::TrayLocation RUNNER_MODE_TL = OgreBites::TL_BOTTOMRIGHT;
    const OgreBites::TrayLocation BOTH_MODES_TL = OgreBites::TL_RIGHT;
    const OgreBites::TrayLocation FPS_TL = OgreBites::TL_BOTTOMLEFT;
    const OgreBites::TrayLocation HELP_PANEL_TL = OgreBites::TL_BOTTOM;
    const OgreBites::TrayLocation DEBUG_PANEL_TL = OgreBites::TL_LEFT;

public:
    HUD(Controller* controller);
    virtual ~HUD();

    /**
     * Update the HUD elements.
     */
    void update(const Ogre::FrameEvent& evt);

    /**
     * Set the params panel (static information).
     */
    void setHelpPanel(Ogre::StringVector params, Ogre::StringVector values);

    // setups
    void setupRunnerMode();
    void setupShooterMode();

    // debug
    void setupDebugOn();
    void setupDebugOff();
};
}

#endif
