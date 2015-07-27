#ifndef _PATHMANAGER_HPP_
#define _PATHMANAGER_HPP_

#include <vector>

#include <OgreVector3.h>
#include <OgreSimpleSpline.h>
#include <ProceduralPathGenerators.h>

namespace Cycleshooter {

class PathManager: public Ogre::SimpleSpline {

    Ogre::Real epsilon = 0.0001;

    //Player
    Ogre::Vector3 currentTangent;
    Ogre::Vector3 lastTangent;
    Ogre::Real startPlayerPosition = Ogre::Real(0.001);
    Ogre::Real parametricPosition; //parametric portion of the whole Spline curve starts on 0.003 and goes to 1
    Ogre::Real VELOCITY_FACTOR = 0.00000001;
    Ogre::Real splineStep = 0.0001;

    //Monster
    Ogre::Vector3 monsterCurrentTangent;
    Ogre::Vector3 monsterLastTangent;
    Ogre::Real startMonsterPosition = Ogre::Real(0);
    Ogre::Real monsterParametricPosition;
    Ogre::Real monsterSplineStep = 0.0005;

    //for debugging
    Procedural::Path proceduralPath;
    Ogre::MeshPtr splineMesh;

    void go(const std::vector<Ogre::Vector3>& controlPoints);

public:
    PathManager();
    PathManager(const char* file);
    PathManager(const std::vector<Ogre::Vector3>& controlPoints);

    void updateTangents();
    void updateParametricPosition();
    void updateSplineStep(double playerVelocity);

    Ogre::Vector3 getCurrentTangent() const;
    Ogre::Vector3 getLastTangent() const;
    Ogre::Vector3 getMonsterCurrentTangent() const;
    Ogre::Vector3 getMonsterLastTangent() const;
};

}

#endif
