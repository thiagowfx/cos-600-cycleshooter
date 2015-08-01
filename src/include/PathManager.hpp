#ifndef _PATHMANAGER_HPP_
#define _PATHMANAGER_HPP_

#include <vector>

#include <OgreVector3.h>
#include <OgreSimpleSpline.h>
#include <ProceduralPathGenerators.h>

#include "Logging.hpp"

namespace Cycleshooter {

class PathManager: public Ogre::SimpleSpline {

    Ogre::Real epsilon = 0.0001;

    // Monster
    Ogre::Vector3 monsterTangent;
    Ogre::Real monsterParametricPosition = 0;
    Ogre::Real monsterSplineStep = 0.001;
    unsigned int monsterIndex = 0;
    unsigned int monsterNextIndex = 1;
    Ogre::Vector3 monsterNextPosition;

    //for debugging
    Procedural::Path proceduralPath;
    Ogre::MeshPtr splineMesh;

    void go(const std::vector<Ogre::Vector3>& controlPoints);

public:
    PathManager(const char* file);
    PathManager(const std::vector<Ogre::Vector3>& controlPoints);

    void monsterPathUpdate();
    void updateIndex();
    void setDebug(bool debug);
    Ogre::Vector3 getMonsterTangent() const;
    Ogre::Vector3 getTangent(unsigned index, double t) const;
    Ogre::Vector3 getAntiTangentFromPoint(const Ogre::Vector3& point) const;
    Ogre::Vector3 getMonsterNextPosition() const;
    void setMonsterNextPosition(const Ogre::Vector3 &value);
};

}

#endif
