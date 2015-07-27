#include "PathManager.hpp"

namespace Cycleshooter {


Ogre::Vector3 PathManager::getCurrentTangent() const {
    return currentTangent;
}

Ogre::Vector3 PathManager::getLastTangent() const {
    return lastTangent;
}

Ogre::Vector3 PathManager::getMonsterCurrentTangent() const {
    return monsterCurrentTangent;
}

Ogre::Vector3 PathManager::getMonsterLastTangent() const {
    return monsterLastTangent;
}

PathManager::PathManager() {
    parametricPosition = startPlayerPosition + epsilon;
    currentTangent = Ogre::Vector3(0,0,-1);
    lastTangent = Ogre::Vector3(0,0,-1);

    monsterParametricPosition = startMonsterPosition + epsilon;
    monsterCurrentTangent = Ogre::Vector3(0,0,-1);
    monsterLastTangent = Ogre::Vector3(0,0,-1);

    //Circular curve for debugging
    this->addPoint(Ogre::Vector3(0,0,0));
    this->addPoint(Ogre::Vector3(0,0,-10000));
    this->addPoint(Ogre::Vector3(10000,0,-10000));
    this->addPoint(Ogre::Vector3(10000,0,0));
    this->addPoint(Ogre::Vector3(0,0,0));

    /*Ogre::SimpleSpline simpleSpline = this->SimpleSpline;
    Procedural::CatmullRomSpline3 catmullSpline = Procedural::CatmullRomSpline3(simpleSpline);
    catmullSpline.close();
    proceduralPath = catmullSpline.realizePath();
    splineMesh = proceduralPath.realizeMesh("splineMesh");*/
}

PathManager::PathManager(std::vector<Ogre::Vector3> controlPoints){
    parametricPosition = startPlayerPosition + epsilon;

    for(int i = 0; i < controlPoints.size(); i++){
        this->addPoint(controlPoints[i]);
    }

}

void PathManager::updateTangents() {
    lastTangent = currentTangent;
    Ogre::Vector3 tangentFirstPoint = this->interpolate(parametricPosition - epsilon);
    Ogre::Vector3 tangentSecondPoint = this->interpolate(parametricPosition);
    currentTangent = tangentSecondPoint - tangentFirstPoint;

    monsterLastTangent = monsterCurrentTangent;
    Ogre::Vector3 monsterTangentFirstPoint = this->interpolate(monsterParametricPosition - epsilon);
    Ogre::Vector3 monsterTangentSecondPoint = this->interpolate(monsterParametricPosition);
    monsterCurrentTangent = monsterTangentSecondPoint - monsterTangentFirstPoint;
}

void PathManager::updateParametricPosition(){
    if (parametricPosition + splineStep < 1){
        parametricPosition += splineStep;
    } else {
        parametricPosition = startPlayerPosition + epsilon;
    }

    if (monsterParametricPosition + monsterSplineStep < 1){
        monsterParametricPosition += monsterSplineStep;
    } else {
        monsterParametricPosition = startMonsterPosition + epsilon;
    }
}

void PathManager::updateSplineStep(double playerVelocity){
    splineStep = VELOCITY_FACTOR * playerVelocity;
}

}
