#ifndef _LOGICMANAGER_HPP_
#define _LOGICMANAGER_HPP_

#include <Ogre.h>

#include "Controller.hpp"
#include "ConfigManager.hpp"
#include "Logging.hpp"

namespace Cycleshooter {
class Controller;

// TODO: add members related to the current difficulty --> terrain and crosshair accuracy
class LogicManager {

    Controller* const controller = NULL;

    Ogre::Viewport *viewportFull = NULL;
    Ogre::Viewport *viewportMirror = NULL;

    Ogre::SceneNode* parentPlayerNode = NULL;
    Ogre::SceneNode* playerNode = NULL;
    Ogre::SceneNode* frontCameraNode = NULL;
    Ogre::SceneNode* rearCameraNode = NULL;
    Ogre::SceneNode* monsterNode = NULL;

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
    int monsterHealth = ConfigManager::instance().getInt("LogicManager.initial_monster_health");

    void decrementMonsterHealth(int quantity = 1);

    /*
     * Player section.
     */

    /**
     * Update the player position according to the passed time.
     */
    void updatePlayerPosition(const Ogre::Real& time);

    /**
     * Update the monster position according to the passed time.
     */
    void updateMonsterPosition(const Ogre::Real& time);

    /**
     * Calculate Bicycle Friction based on terrain properties.
     */
    int calculateFriction(int terrainAt);

    /**
     * Return true if the monster is near enough in relation to the player.
     */
    bool checkPlayerMonsterCollision();

    /**
     * Number of bullets available for the player.
     */
    int playerAmmo = ConfigManager::instance().getInt("LogicManager.initial_player_ammo");

    /**
     * Add a bullet to the total ammo of the player.
     */
    void incrementPlayerAmmo(int quantity = 1);

    /**
     * Remove a bullet from the total ammo of the player. Return false if out of ammo before this operation.
     */
    bool decrementPlayerAmmo(int quantity = 1);

    // go
    void go();
    void createCameras();
    void createSceneNodes();
    void createViewports();
    void createRtt();

    // customizable settings
    const double CAMERA_NEAR_CLIP_DISTANCE = ConfigManager::instance().getDouble("LogicManager.camera_near_clip_distance");
    const double CAMERA_FAR_CLIP_DISTANCE = ConfigManager::instance().getDouble("LogicManager.camera_far_clip_distance");
    const double MIRROR_PERCENTAGE_H = ConfigManager::instance().getDouble("LogicManager.mirror_percentage_h");
    const double MIRROR_PERCENTAGE_V = ConfigManager::instance().getDouble("LogicManager.mirror_percentage_v");

    // Data structure for difficulty mapping.
    std::vector<float> difficultyParamenter;

    // Rotation variables
    double angularVelocity;
    // TODO: create a config out of this hard-coded constant
    Ogre::Degree MAX_ANGLE = Ogre::Degree(45);
    double ROTATION_FACTOR = 1.0f;
    double MONSTER_STEP = 1.0f;
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
    void setDebug(bool debug);

    //Move functions.
    //void translateMonster(int difficulty, Ogre::Vector3 translation);
    void yawCamera();
    void rotateCamera(const Ogre::Degree& angle, const Ogre::Vector3& pathDirection, const Ogre::Vector3& lastPathDirection);
    void updateMonster(const Ogre::Vector3& tangent,const Ogre::Vector3& monsterNextPosition);
    double getDistanceToMonster() const;

    // getters and setters
    Ogre::SceneNode *getPlayerNode() const;
    int getPlayerAmmo() const;
    int getMonsterHealth() const;
    void setDifficultyParamenter();
    double getAngularVelocity() const;
    void setAngularVelocity(double value);
};
}

#endif
