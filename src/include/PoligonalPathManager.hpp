#ifndef _PoligonalPathManager_HPP_
#define _PoligonalPathManager_HPP_

#include <OgreVector3.h>

namespace Cycleshooter {
class PoligonalPathManager {


    int playerPointIndex;
    int playerNextPointIndex;
    Ogre::Vector3 playerCurrentTangent;
    Ogre::Vector3 playerLastTangent;

    int monsterPointIndex;
    int monsterNextPointIndex;
    Ogre::Vector3 monsterCurrentTangent;
    Ogre::Vector3 monsterLastTangent;
public:

    std::vector<Ogre::Vector3> controlPoints;

    PoligonalPathManager();
    PoligonalPathManager(const char* file);

    void setDebug(bool debug);

    virtual ~PoligonalPathManager();

    void go(const std::vector<Ogre::Vector3>& controlPoints);
    void updatePlayerPoint(Ogre::Vector3 playerPosition);
    void updateMonsterPoint(Ogre::Vector3 monsterPosition);
    void updateTangents();
    Ogre::Vector3 getPlayerCurrentTangent() const;
    Ogre::Vector3 getPlayerLastTangent() const;
    Ogre::Vector3 getMonsterCurrentTangent() const;
    Ogre::Vector3 getMonsterLastTangent() const;
};
}

#endif
