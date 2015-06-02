#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <iostream>

namespace Cycleshooter {

// TODO: I think this class should be concerned only with the player position
// and logic to move and update it over time. Why should we track the heart rate
// and the ammo here, if it is proxied on the LogicManager anyway?
// the same for the monster health and similar
class Player {
    /**
     * Player heart rate.
     * Should simulate or acquire a real human heart rate.
     */
    unsigned heartRate;

    /**
     * Number of bullets available for the player.
     */
    // TODO: change this later on, maybe with a (debug) button to manually increase available bullets
    // or just use a very high value. Or, better yet, add debugOn and debugOff methods to logic manager
    // a debug on should increase the available ammo for the player
    unsigned ammo = 30;

    sf::Vector3<double> bicycleSpeed;
    sf::Vector3<double> playerPosition;

    // TODO: shouldn't this be on the Crosshair Or logic manager class?
    double sensibility;

    // TODO: Do we really need this here since we have a CrosshairManager class now?
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
