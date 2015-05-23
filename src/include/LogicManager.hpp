#ifndef _LOGICMANAGER_HPP_
#define _LOGICMANAGER_HPP_

#include <Player.hpp>
#include <Monster.hpp>

namespace Cycleshooter {
class Controller;

class LogicManager {
    Player *player;
    Monster *monster;
public:

    LogicManager();
    virtual ~LogicManager();

    Player *getPlayer() const;
    void setPlayer(Player *value);
    Monster *getMonster() const;
    void setMonster(Monster *value);
};
}

#endif
