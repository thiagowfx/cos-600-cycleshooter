#include "Player.hpp"

namespace Cycleshooter {


void Player::setSensibility(double value)
{
    sensibility = value;
}

unsigned Player::getHeartRate() const
{
    return heartRate;
}
Player::Player() {
}

Player::~Player() {
}

}
