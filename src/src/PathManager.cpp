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
    parametricPosition = startPlayerPosition + epsilon;
    currentTangent = Ogre::Vector3(0,0,-1);
    lastTangent = Ogre::Vector3(0,0,-1);

    monsterParametricPosition = startMonsterPosition + epsilon;
    monsterCurrentTangent = Ogre::Vector3(0,0,-1);
    monsterLastTangent = Ogre::Vector3(0,0,-1);

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

}
