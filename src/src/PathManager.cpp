#include "PathManager.hpp"

namespace Cycleshooter {


Ogre::Vector3 PathManager::getCurrentTangent() const {
    return currentTangent;
}

Ogre::Vector3 PathManager::getLastTangent() const {
    return lastTangent;
}
PathManager::PathManager() {
    t = 3 * increment;

    //Circular curve for debugging
    this->addPoint(Ogre::Vector3(0,0,0));
    this->addPoint(Ogre::Vector3(0,0,-10000));
    this->addPoint(Ogre::Vector3(10000,0,-10000));
    this->addPoint(Ogre::Vector3(10000,0,0));
    this->addPoint(Ogre::Vector3(0,0,0));
}

void PathManager::updateTangents() {
    Ogre::Vector3 lTangent_firstPoint = this->interpolate(t - 3 * increment);
    Ogre::Vector3 lTangent_secondPoint = this->interpolate(t - 2 * increment);
    lastTangent = lTangent_secondPoint - lTangent_firstPoint;
    Ogre::Vector3 cTangent_firstPoint = this->interpolate(t - increment);
    Ogre::Vector3 cTangent_secondPoint = this->interpolate(t);
    currentTangent = cTangent_secondPoint - cTangent_firstPoint;
}

void PathManager::updateT(){
    if (t + increment < 1){
        t += increment;
    }
    else {
        t = 3 * increment;
    }
}

}
