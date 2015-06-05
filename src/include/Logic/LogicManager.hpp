#ifndef _LOGICMANAGER_HPP_
#define _LOGICMANAGER_HPP_

#include <Ogre.h>

#include "Controller.hpp"
#include "Player.hpp"
#include "Monster.hpp"

namespace Cycleshooter {
class Controller;

// TODO: add members related to the current difficulty --> terrain and crosshair accuracy
class LogicManager {

    Controller* const controller = NULL;

    std::unique_ptr<Player> player;
    std::unique_ptr<Monster> monster;

    Ogre::Viewport *viewportFull = NULL;
    Ogre::Viewport *viewportMirror = NULL;

    Ogre::SceneNode* parentPlayerSceneNode = NULL;
    Ogre::SceneNode* frontPlayerSceneNode = NULL;
    Ogre::SceneNode* rearPlayerSceneNode = NULL;

    Ogre::Camera *frontCamera = NULL;
    Ogre::Camera *rearCamera = NULL;

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
     * Update the game logic overall.
     */
    void update(const Ogre::FrameEvent &evt);

    /**
     * Fires a shot.
     * Not intended to be used as an automatic or semi-automatic weapon:
     * Its meaning is more like a sniper rifle shot.
     */
    void shoot();

    // getters and setters
    Player *getPlayer() const;
    Monster *getMonster() const;
    Ogre::SceneNode *getPlayerSceneNode() const;

    // setups
    void setupRunnerMode();
    void setupShooterMode();

    // debug
    void setDebugOn();
    void setDebugOff();

    // methods for decoupling purposes (proxy methods)
    unsigned getHeartRate() const;
    void setHeartRate(const unsigned& value);
    unsigned getAmmo() const;
};
}

#endif
