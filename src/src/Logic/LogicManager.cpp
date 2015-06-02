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

LogicManager::LogicManager(Controller* controller) :
    controller(controller)
{
    std::cout << "LogicManager: Constructor" << std::endl;

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

void LogicManager::shoot() {
    std::cout << "LogicManager: shoot" << std::endl;

    if(player->getAmmo() > 0) {
        AudioManager::instance().random_play_shoot();
        player->decrementAmmo();

        // TODO: (maybe) replenish ammo in the map / terrain / collision part?
        // TODO: create singleton Audio class to manage (1) game music
        // TODO: create singleton Audio class to manage (2) game sounds
        // TODO: add several sound effects for each outcome
        // TODO: RTT crosshair + monster logic
        // TODO: if hit, then decrease monster life
        // TODO: check if monster is dead
    }
    else {
        std::cout << " |-> No more ammo" << std::endl;
    }
}

}

