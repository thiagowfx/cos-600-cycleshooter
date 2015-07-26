#include "PathManager.hpp"

namespace Cycleshooter {


Ogre::Vector3 PathManager::getCurrentTangent() const {
    return currentTangent;
}

Ogre::Vector3 PathManager::getLastTangent() const {
    return lastTangent;
}
PathManager::PathManager() {
    parametricPosition = 3 * epsilon + startPlayerPosition;

    //Circular curve for debugging
    this->addPoint(Ogre::Vector3(0,0,0));
    this->addPoint(Ogre::Vector3(0,0,-10000));
    this->addPoint(Ogre::Vector3(10000,0,-10000));
    this->addPoint(Ogre::Vector3(10000,0,0));
    this->addPoint(Ogre::Vector3(0,0,0));
}

PathManager::PathManager(std::vector<Ogre::Vector3> controlPoints){
    parametricPosition = 3 * epsilon + startPlayerPosition;

    for(int i = 0; i < controlPoints.size(); i++){
        this->addPoint(controlPoints[i]);
    }

}

void PathManager::updateTangents() {
    Ogre::Vector3 lTangent_firstPoint = this->interpolate(parametricPosition - 3 * epsilon);
    Ogre::Vector3 lTangent_secondPoint = this->interpolate(parametricPosition - 2 * epsilon);
    lastTangent = lTangent_secondPoint - lTangent_firstPoint;
    Ogre::Vector3 cTangent_firstPoint = this->interpolate(parametricPosition - epsilon);
    Ogre::Vector3 cTangent_secondPoint = this->interpolate(parametricPosition);
    currentTangent = cTangent_secondPoint - cTangent_firstPoint;
}

void PathManager::updateParametricPosition(){
    if (parametricPosition + splineStep < 1){
        parametricPosition += splineStep;
    }
    else {
        parametricPosition = 3 * splineStep;
    }
}

void PathManager::updateSplineStep(double playerVelocity){
    splineStep = VELOCITY_FACTOR * playerVelocity;
}

}
