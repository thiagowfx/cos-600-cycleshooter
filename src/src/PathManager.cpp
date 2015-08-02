#include "PathManager.hpp"

namespace Cycleshooter {

Ogre::Vector3 PathManager::getMonsterTangent() const {
    return monsterTangent;
}

Ogre::Vector3 PathManager::getTangent(unsigned index, double t) const {
    if ((t + epsilon) > 1.0) {
        return (mTangents[(index + 1) % mPoints.size()]).normalisedCopy();
    }
    else {
        Ogre::Vector3 firstPoint = interpolate(index, t);
        Ogre::Vector3 secondPoint =  interpolate(index, t + epsilon);
        return (firstPoint - secondPoint).normalisedCopy();
    }
}

Ogre::Vector3 PathManager::getAntiTangentFromPoint(const Ogre::Vector3 &point) const {
    std::pair<unsigned,double> target = binSearchNearestPointOnSpline(point);
    return getTangent(target.first, target.second);
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


Ogre::Vector3 PathManager::getMonsterNextPosition() const{
    return monsterNextPosition;
}

void PathManager::setMonsterNextPosition(const Ogre::Vector3 &value) {
    monsterNextPosition = value;
}

std::pair<unsigned,double> PathManager::binSearchNearestPointOnSpline(const Ogre::Vector3 &point) const {
    int nearestPointIndex;
    double squaredDistance = std::numeric_limits<double>::max();

    for(unsigned i = 0; i < mPoints.size(); ++i) {
        double d = point.squaredDistance(mPoints[i]);
        if(d < squaredDistance) {
            squaredDistance = d;
            nearestPointIndex = i;
        }
    }

    int previousIndex = (nearestPointIndex + mPoints.size() - 1) % mPoints.size();
    int nextIndex = (nearestPointIndex + 1) % mPoints.size();

    auto binSearch = [&](int leftIndex, int rightIndex, const int NUM_STEPS = 10) {
        int step;
        double tbegin, tavg, tend;
        double dbegin, dend, davg;

        for(step = 0,
            tbegin = 0.0,
            tend = 1.0,
            dbegin = point.squaredDistance(mPoints[leftIndex]),
            dend = point.squaredDistance(mPoints[rightIndex])
            ; step < NUM_STEPS; ++step) {

            tavg = (tbegin + tend) / 2.0;
            davg = point.squaredDistance(interpolate(leftIndex, tavg));

            if(dbegin < davg) {
                tend = tavg;
                dend = davg;
            }
            else {
                tbegin = tavg;
                dbegin = davg;
            }
        }

        return std::pair<double,double>(tavg, davg);
    };

    std::pair<double,double> bse1 = binSearch(previousIndex, nearestPointIndex);
    std::pair<double,double> bse2 = binSearch(nearestPointIndex, nextIndex);

    int targetIndex = (bse1.second < bse2.second) ? previousIndex : nearestPointIndex;
    double targetT = (bse1.second < bse2.second) ? bse1.first : bse2.first;

    return std::pair<unsigned,double>(targetIndex, targetT);
}

void PathManager::go(const std::vector<Ogre::Vector3>& controlPoints) {
    for(int i = 0; i < controlPoints.size(); i++){
        this->addPoint(controlPoints[i]);
    }
}

void PathManager::monsterPathUpdate(Ogre::Real timeSinceLastFrame, const Ogre::Vector3& playerPosition, const Ogre::Vector3& monsterPosition) {
    if(!isPlayerClose){
        monsterSplineStep = monsterSplineVelocity * timeSinceLastFrame;
        if(monsterParametricPosition + monsterSplineStep < 1.0){
            monsterParametricPosition += monsterSplineStep;
        }
        else {
            monsterParametricPosition = 0.0;
            updateIndex();
        }
        Ogre::Vector3 monsterTangentFirstPoint = this->interpolate(monsterIndex, monsterParametricPosition);
        Ogre::Vector3 monsterTangentSecondPoint = this->interpolate(monsterIndex, monsterParametricPosition + epsilon);
        setMonsterNextPosition(monsterTangentFirstPoint);
        //LOG("index = %d, nextIndex = %d",monsterIndex,monsterNextIndex);

        monsterTangent = monsterTangentSecondPoint - monsterTangentFirstPoint;
        monsterTangent.normalise();
        if((playerPosition - monsterPosition).length() < MINIMUM_DISTANCE){
            AudioManager::instance().playSound(SOUND_MONSTER_PROXIMITY);
        }
    }
    else {
        monsterTangent = playerPosition - monsterPosition;
        monsterTangent.normalise();
        Ogre::Vector3 distance = monsterVelocityIfCloseToPlayer * timeSinceLastFrame * monsterTangent;
        setMonsterNextPosition(monsterNextPosition + distance);
        if ((playerPosition - monsterPosition).length() < MINIMUM_DISTANCE) { //player escaped
            std::pair<unsigned,double> indexParameter = binSearchNearestPointOnSpline(monsterPosition);
            monsterIndex = indexParameter.first;
            monsterParametricPosition = indexParameter.second;
        }
    }
    //update condition
    isPlayerClose = (playerPosition - monsterPosition).length() < MINIMUM_DISTANCE ? true: false;
}


void PathManager::updateIndex(){
    monsterIndex = monsterNextIndex;
    monsterNextIndex = (monsterNextIndex + 1) % mPoints.size();
    LOG("Monster index change to %d", monsterNextIndex);
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
