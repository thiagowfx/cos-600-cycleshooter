#ifndef _LOGICMANAGER_HPP_
#define _LOGICMANAGER_HPP_

#include <OgreFrameListener.h>

#include "Controller.hpp"
#include "Player.hpp"
#include "Monster.hpp"

namespace Cycleshooter {
class Controller;

// TODO: add members related to the current difficulty --> terrain and crosshair accuracy
class LogicManager {
    Controller* const controller;
    Player *player;
    Monster *monster;

public:
    LogicManager(Controller* controller);
    virtual ~LogicManager();

    /**
     * Update the game logic overall.
     */
    void update(const Ogre::FrameEvent &evt);

    /**
     * Fires a shot.
     * Not intended to be used as an automatic or semi-automatic weapon:
     * It should be interpreted more like a sniper rifle shot.
     */
    void shoot();

    // getters and setters
    Player *getPlayer() const;
    Monster *getMonster() const;

    // methods for decoupling purposes (proxy methods)
    unsigned getHeartRate() const;
    void setHeartRate(const unsigned& value);
    unsigned getAmmo() const;
};
}

#endif
