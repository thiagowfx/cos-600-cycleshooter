#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace Cycleshooter {

class Player {
    unsigned heartRate;

    sf::Vector3<double> bicycleSpeed;
    sf::Vector3<double> playerPosition;
    unsigned ammo;
    double sensibility;
    sf::Vector2<double> crosshairPosition;

public:
    Player();

    // getters and setters
    unsigned getHeartRate() const;
    void setHeartRate(const unsigned &value);
};
}

#endif
