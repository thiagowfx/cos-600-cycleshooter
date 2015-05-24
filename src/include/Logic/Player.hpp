#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace Cycleshooter {

class Player {
    unsigned heartRate;

    /**
     * Number of bullets available for the player.
     */
    unsigned ammo = 0;

    sf::Vector3<double> bicycleSpeed;
    sf::Vector3<double> playerPosition;
    double sensibility;
    sf::Vector2<double> crosshairPosition;

public:
    Player();

    // getters and setters
    unsigned getHeartRate() const;
    void setHeartRate(const unsigned &value);
    unsigned getAmmo() const;
    void setAmmo(const unsigned &value);

    /**
     * Fires a shot.
     */
    void shoot();

    /**
     * Add a bullet to the total ammo of the player.
     */
    void incrementAmmo();
};
}

#endif
