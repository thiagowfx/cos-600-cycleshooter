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

    //Fake path
    Ogre::Real VELOCITY_FACTOR = 0.00005;
    Ogre::Vector3 fakePathTangent;
    Ogre::Real fakePathSplineParameter = 0;
    unsigned int fakePathIndex = 1;
    unsigned int fakePathNextIndex = 2;
    Ogre::Vector3 fakePathNextPosition;

    //Monster
    Ogre::Vector3 monsterTangent;
    Ogre::Real monsterSplineParameter = 0;
    unsigned int monsterIndex = 0;
    unsigned int monsterNextIndex = 1;
    double monsterSplineStep = 0.001;
    Ogre::Vector3 monsterNextPosition;

    //for debugging
    Procedural::Path proceduralPath;
    Ogre::MeshPtr splineMesh;

    void go(const std::vector<Ogre::Vector3>& controlPoints);

public:
    PathManager();
    PathManager(const char* file);
    PathManager(const std::vector<Ogre::Vector3>& controlPoints);

    void monsterPathUpdate();
    void updateIndex();

    void fakePathUpdate();
    void fakePathSplineStepUpdate(const double& playerVelocity, Ogre::Real time);
    void fakePathUpdateIndex();

    std::vector<Ogre::Real> parametricValue(Ogre::Vector3 splinePoint, unsigned int fromIndex);


    void setDebug(bool debug);

    Ogre::Vector3 getMonsterTangent() const;
    Ogre::Vector3 getMonsterNextPosition() const;
    void setMonsterNextPosition(const Ogre::Vector3 &value);
    Ogre::Vector3 getFakePathTangent() const;
    Ogre::Vector3 getFakePathNextPosition() const;
};

}

#endif
