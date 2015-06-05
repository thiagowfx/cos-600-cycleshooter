#ifndef _LOGICMANAGER_HPP_
#define _LOGICMANAGER_HPP_

#include <Ogre.h>

#include "Controller.hpp"

namespace Cycleshooter {
class Controller;

// TODO: add members related to the current difficulty --> terrain and crosshair accuracy
class LogicManager {

    Controller* const controller = NULL;

    Ogre::Viewport *viewportFull = NULL;
    Ogre::Viewport *viewportMirror = NULL;

    Ogre::SceneNode* parentPlayerSceneNode = NULL;
    Ogre::SceneNode* frontPlayerSceneNode = NULL;
    Ogre::SceneNode* rearPlayerSceneNode = NULL;

    Ogre::Camera *frontCamera = NULL;
    Ogre::Camera *rearCamera = NULL;

    /*
     * Player section.
     */

    /**
     * Player heart rate.
     * Should simulate or acquire a real human heart rate.
     */
    int playerHeartRate;

    /**
     * Number of bullets available for the player.
     */
    int playerAmmo = 0;

    /**
     * Add a bullet to the total ammo of the player.
     */
    void incrementPlayerAmmo(int quantity = 1);

    /**
     * Remove a bullet from the total ammo of the player.
     */
    void decrementPlayerAmmo(int quantity = 1);

    // go
    void go();
    void createCameras();
    void createSceneNodes();
    void createViewports();

    // customizable settings
    const double CAMERA_NEAR_CLIP_DISTANCE = 5.0;
    const double CAMERA_FAR_CLIP_DISTANCE = 10000.0;
    const double MIRROR_PERCENTAGE_H = 0.675;
    const double MIRROR_PERCENTAGE_V = 0.135;

public:
    LogicManager(Controller* controller);

    /**
     * Update the game logic (overall).
     */
    void update(const Ogre::FrameEvent &evt);

    /**
     * Fires a shot.
     * Not intended to be used as an automatic or semi-automatic weapon:
     * Its meaning is more like a sniper rifle shot.
     */
    void shoot();

    // getters and setters
    Ogre::SceneNode *getPlayerSceneNode() const;
    int getPlayerHeartRate() const;
    void setPlayerHeartRate(const int& value);
    int getPlayerAmmo() const;

    // setups
    void setupRunnerMode();
    void setupShooterMode();

    // debug
    void setDebugOn();
    void setDebugOff();
};
}

#endif
