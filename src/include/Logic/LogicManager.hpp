#ifndef _LOGICMANAGER_HPP_
#define _LOGICMANAGER_HPP_

#include <OgreFrameListener.h>

#include "Controller.hpp"
#include "Player.hpp"
#include "Monster.hpp"

namespace Cycleshooter {
class Controller;

class LogicManager {
    Controller* controller;
    Player *player;
    Monster *monster;

public:
    LogicManager(Controller* controller);
    virtual ~LogicManager();

    /**
     * Update the game logic.
     */
    void update(const Ogre::FrameEvent &evt);

    /**
     * Fires a shot.
     */
    void shoot();

    // getters and setters
    Player *getPlayer() const;
    Monster *getMonster() const;

    // methods for decoupling purposes
    unsigned getHeartRate() const;
    void setHeartRate(const unsigned& value);
    unsigned getAmmo() const;
};
}

#endif
