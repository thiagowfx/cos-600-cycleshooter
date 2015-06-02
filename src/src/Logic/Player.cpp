#include "Player.hpp"

namespace Cycleshooter {

unsigned Player::getHeartRate() const {
    return heartRate;
}

void Player::setHeartRate(const unsigned &value) {
    heartRate = value;
}

unsigned Player::getAmmo() const {
    return ammo;
}

void Player::setAmmo(const unsigned &value) {
    ammo = value;
}

void Player::incrementAmmo() {
    std::cout << "Player: incrementing ammo" << std::endl;

    ++ammo;
}

void Player::decrementAmmo() {
    std::cout << "Player: decrementing ammo" << std::endl;

    --ammo;
}

Player::Player() {
}

}
