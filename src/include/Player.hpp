#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include <vector>

namespace Cycleshooter {
class GameLogic;

class Player {
    unsigned heartRate;
    std::vector<double> bicycleVelocity;
    std::vector<int> playerPosition;
    int possessedBullets;
    double sensibility;
    std::pair<double, double> crossPosition;

public:
    Player();
    virtual ~Player();

    void setSensibility(double value);
    unsigned getHeartRate() const;
};
}

#endif
