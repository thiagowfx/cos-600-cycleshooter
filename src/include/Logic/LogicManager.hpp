#ifndef _LOGICMANAGER_HPP_
#define _LOGICMANAGER_HPP_

#include <OgreFrameListener.h>

#include "Player.hpp"
#include "Monster.hpp"

namespace Cycleshooter {
class Controller;

class LogicManager {
    Player *player;
    Monster *monster;

public:
    LogicManager();
    virtual ~LogicManager();

    /**
     * Update the game logic.
     */
    void update(const Ogre::FrameEvent &evt);

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
