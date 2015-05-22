#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

namespace Cycleshooter {
class GameLogic;

class Player {
    double cycleVelocityX;    //gets from ergometric bicycle * sin(angle between previous lookAt and actual lookAt)
    double cycleVelocityZ;    //gets from ergometric bicycle * cos(angle between previous lookAt and actual lookAt)
    double cycleVelocityY;    //probably always = 0
    double worldPositionX;    //position coordinates in the Ogre world
    double worldPositionY;
    double worldPositionZ;
    int possessedBullets;
    double crossPositionX;
    double crossPositionY;

public:
    Player();
    virtual ~Player();

    void turnRightCycleMode();
    void turnLeftCycleMode();
    void pressRightShooterMode();
    void pressLeftShooterMode();
    void pressUpShooterMode();
    void pressDownShooterMode();
};
}

#endif
