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

    Ogre::SceneNode* parentPlayerNode = NULL;
    Ogre::SceneNode* frontPlayerNode = NULL;
    Ogre::SceneNode* rearPlayerNode = NULL;

    Ogre::Camera *frontCamera = NULL;
    Ogre::Camera *rearCamera = NULL;

    /*
     *  RTT.
     */
    Ogre::TexturePtr rttTexture;
    Ogre::RenderTexture* rttRenderTarget = NULL;

    /*
     *  Monster section.
     */
    int monsterHealth = 10;

    void decrementMonsterHealth(int quantity = 1);

    /*
     * Player section.
     */

    /**
     * Bicycle speed.
     * Should simulate or acquire a real bike speed.
     */
    double speed = 10 * 10.0;

    /**
     * Update the player position according to the passed time.
     */
    void updatePlayerPosition(const double& time);

    /**
     * Number of bullets available for the player.
     */
    int playerAmmo = 10;

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
    void createRtt();

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

    // setups
    void setupRunnerMode();
    void setupShooterMode();

    // debug
    void setDebugOn();
    void setDebugOff();

    // getters and setters
    Ogre::SceneNode *getPlayerNode() const;
    int getPlayerAmmo() const;
    int getMonsterHealth() const;
    double getSpeed() const;
};
}

#endif
