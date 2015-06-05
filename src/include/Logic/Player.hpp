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

    sf::Vector3<double> bicycleSpeed;
    sf::Vector3<double> playerPosition;

    // TODO: shouldn't this be on the Crosshair Or logic manager class?
    double sensibility;

    // TODO: Do we really need this here since we have a CrosshairManager class now?
    sf::Vector2<double> crosshairPosition;

public:
    Player();
};
}

#endif
