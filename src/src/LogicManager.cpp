#include "LogicManager.hpp"

namespace Cycleshooter {


Player *LogicManager::getPlayer() const
{
    return player;
}

void LogicManager::setPlayer(Player *value)
{
    player = value;
}

Monster *LogicManager::getMonster() const
{
    return monster;
}

void LogicManager::setMonster(Monster *value)
{
    monster = value;
}
LogicManager::LogicManager() {
    player = new Player();
    monster = new Monster();
}

LogicManager::~LogicManager() {
}

}

