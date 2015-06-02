#include "LogicManager.hpp"

namespace Cycleshooter {

Player *LogicManager::getPlayer() const {
    return player;
}

Monster *LogicManager::getMonster() const {
    return monster;
}

unsigned LogicManager::getHeartRate() const {
    return player->getHeartRate();
}

void LogicManager::setHeartRate(const unsigned &value) {
    player->setHeartRate(value);
}

unsigned LogicManager::getAmmo() const {
    return player->getAmmo();
}

LogicManager::LogicManager() {
    player = new Player();
    monster = new Monster();
}

LogicManager::~LogicManager() {
    if(monster)
        delete monster;

    if(player)
        delete player;
}

void LogicManager::update(const Ogre::FrameEvent &evt) {
    // TODO: populate this method
}

}

