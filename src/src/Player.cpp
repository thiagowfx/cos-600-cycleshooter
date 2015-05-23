#include "Player.hpp"

namespace Cycleshooter {

unsigned Player::getHeartRate() const {
    return heartRate;
}

void Player::setHeartRate(const unsigned &value) {
    heartRate = value;
}

Player::Player() {
}

}
