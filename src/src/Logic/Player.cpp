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

Player::Player() {
}

}
