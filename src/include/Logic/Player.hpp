#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <iostream>

namespace Cycleshooter {

class Player {
    unsigned heartRate;

    /**
     * Number of bullets available for the player.
     */
    unsigned ammo = 3;

    sf::Vector3<double> bicycleSpeed;
    sf::Vector3<double> playerPosition;
    double sensibility;
    sf::Vector2<double> crosshairPosition;

public:
    Player();

    /**
     * Add a bullet to the total ammo of the player.
     */
    void incrementAmmo();

    /**
     * Remove a bullet from the total ammo of the player.
     */
    void decrementAmmo();

    // getters and setters
    unsigned getHeartRate() const;
    void setHeartRate(const unsigned &value);
    unsigned getAmmo() const;
    void setAmmo(const unsigned &value);
};
}

#endif
