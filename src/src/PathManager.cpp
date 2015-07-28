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

    //Circular curve for debugging
    /*this->addPoint(Ogre::Vector3(0,0,0));
    this->addPoint(Ogre::Vector3(0,0,-10000));
    this->addPoint(Ogre::Vector3(10000,0,-10000));
    this->addPoint(Ogre::Vector3(10000,0,0));
    this->addPoint(Ogre::Vector3(0,0,0));*/

    /*Ogre::SimpleSpline simpleSpline = this->SimpleSpline;
    Procedural::CatmullRomSpline3 catmullSpline = Procedural::CatmullRomSpline3(simpleSpline);
    catmullSpline.close();
    proceduralPath = catmullSpline.realizePath();
    splineMesh = proceduralPath.realizeMesh("splineMesh");*/
}

PathManager::PathManager(const char *file) {
    std::ifstream ifs(file);
    std::string line;
    std::vector<Ogre::Vector3> controlPoints;

    while(std::getline(ifs, line)) {
        std::stringstream ss;
        ss << line;
        double x, y, z;
        ss >> x >> y >> z;
        controlPoints.push_back(Ogre::Vector3(x, y, z));
    }

    ifs.close();
    go(controlPoints);
}

PathManager::PathManager(const std::vector<Ogre::Vector3>& controlPoints){
    go(controlPoints);
}

void PathManager::go(const std::vector<Ogre::Vector3>& controlPoints) {

    for(int i = 0; i < controlPoints.size(); i++){
        this->addPoint(controlPoints[i]);
    }
    //std::cout << "=================initial monster: " << this->mPoints[0] << std::endl;
}

void PathManager::updateTangents(Ogre::Vector3 monsterPosition) {
    lastTangent = currentTangent;
    //Ogre::Vector3 tangentFirstPoint = this->interpolate(parametricPosition - epsilon);
    //Ogre::Vector3 tangentSecondPoint = this->interpolate(parametricPosition);
    //currentTangent = tangentSecondPoint - tangentFirstPoint;
    std::cout << "Monster position = " << monsterPosition << std::endl;
    monsterLastTangent = monsterCurrentTangent;
    //Ogre::Real t = parametricValue(monsterPosition,monsterIndex)[1];
    t += 0.0001;
    Ogre::Vector3 monsterTangentFirstPoint = this->interpolate(monsterIndex, t);
    Ogre::Vector3 monsterTangentSecondPoint = this->interpolate(monsterIndex, t + epsilon);
    monsterCurrentTangent = this->mPoints[monsterNextIndex] - this->mPoints[monsterIndex];
    //monsterCurrentTangent = monsterTangentSecondPoint - monsterTangentFirstPoint;
    monsterCurrentTangent.normalise();
}


void PathManager::updateMonsterIndex(Ogre::Vector3 monsterPosition){
    if(monsterPosition == this->mPoints[monsterNextIndex]){
        monsterIndex = monsterNextIndex;
        if(monsterNextIndex == this->mPoints.size() - 1){
            monsterNextIndex = 0;
        }
        else {
            monsterNextIndex += 1;
        }
    }
}

void PathManager::updateSplineStep(double playerVelocity){
    splineStep = VELOCITY_FACTOR * playerVelocity;
}

void PathManager::setDebug(bool debug) {
    Ogre::SceneManager* sceneManager = Ogre::Root::getSingleton().getSceneManager("sceneManager");
    static bool firstTimeInit = true;

    if(firstTimeInit) {
        firstTimeInit = !firstTimeInit;
        Ogre::SceneNode* debugPathManagerSceneNode = sceneManager->getRootSceneNode()->createChildSceneNode("debugPathManagerSceneNode");

        for(const auto& point: mPoints) {
            const double DEBUG_NODE_SCALE = 0.25;
            Ogre::Entity* entity = sceneManager->createEntity("sphere.mesh");
            entity->setMaterialName("Cycleshooter/Matheus");
            Ogre::SceneNode* sceneNode = debugPathManagerSceneNode->createChildSceneNode();
            sceneNode->translate(point);
            sceneNode->scale(Ogre::Vector3(DEBUG_NODE_SCALE, DEBUG_NODE_SCALE, DEBUG_NODE_SCALE));
            sceneNode->attachObject(entity);
        }
    }

    sceneManager->getSceneNode("debugPathManagerSceneNode")->setVisible(debug);
}

std::vector<Ogre::Real> PathManager::parametricValue(Ogre::Vector3 splinePoint, unsigned int fromIndex){
    const Ogre::Vector3& point1 = this->mPoints[fromIndex];
    const Ogre::Vector3& point2 = this->mPoints[fromIndex+1];
    const Ogre::Vector3& tan1 = this->mTangents[fromIndex];
    const Ogre::Vector3& tan2 = this->mTangents[fromIndex+1];
    if (splinePoint == point2){
        std::vector<Ogre::Real> result = {1,1,1};
        return result;
    }
    std::vector<Ogre::Real> splineCandidates;
    std::vector<double>a;
    std::vector<double>b;
    std::vector<double>c;
    std::vector<double>d;
    std::vector<double>e;
    a.push_back(2*point1.x-2*point2.x+tan1.x+tan2.x);
    a.push_back(2*point1.y-2*point2.y+tan1.y+tan2.y);
    a.push_back(2*point1.z-2*point2.z+tan1.x+tan2.z);
    b.push_back((-3)*point1.x+3*point2.x-2*tan1.x-tan2.x);
    b.push_back((-3)*point1.y+3*point2.y-2*tan1.y-tan2.y);
    b.push_back((-3)*point1.z+3*point2.z-2*tan1.z-tan2.z);
    c.push_back(tan1.x);
    c.push_back(tan1.y);
    c.push_back(tan1.z);
    d.push_back(point1.x);
    d.push_back(point1.y);
    d.push_back(point1.z);
    e.push_back(splinePoint.x);
    e.push_back(splinePoint.y);
    e.push_back(splinePoint.z);
    for(int i = 0; i < 3; i++){
        double x = -27*(d[i]-e[i])*pow(a[i],2)+9*a[i]*b[i]*c[i]-2*std::pow(b[i],3);
        double y = 4*std::pow(3*a[i]*c[i]-std::pow(b[i],2),3);
        double z = std::pow(std::sqrt(x*x*y)-x,1/3);
        double t = z/(3*a[i]*std::pow(2,1/3)) - std::pow(2,1/3)*(3*a[i]*c[i]-std::pow(b[i],2))/(3*a[i]*z) - b[i]/(3*a[i]);
        splineCandidates.push_back(t);
    }
    return splineCandidates;
}

}
