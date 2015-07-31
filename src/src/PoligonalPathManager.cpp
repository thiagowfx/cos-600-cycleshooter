#include "PoligonalPathManager.hpp"

namespace Cycleshooter {


Ogre::Vector3 PoligonalPathManager::getPlayerCurrentTangent() const {
    return playerCurrentTangent;
}

Ogre::Vector3 PoligonalPathManager::getPlayerLastTangent() const {
    return playerLastTangent;
}

Ogre::Vector3 PoligonalPathManager::getMonsterCurrentTangent() const {
    return monsterCurrentTangent;
}

Ogre::Vector3 PoligonalPathManager::getMonsterLastTangent() const {
    return monsterLastTangent;
}
PoligonalPathManager::PoligonalPathManager() {
}

PoligonalPathManager::PoligonalPathManager(const char *file) {
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

void PoligonalPathManager::go(const std::vector<Ogre::Vector3>& controlPoints){
    monsterPointIndex = 0;
    monsterNextPointIndex = 1;
    playerPointIndex = 1;
    playerNextPointIndex = 2;
    for(int i = 0; i < controlPoints.size(); i++){
        this->controlPoints.push_back(controlPoints[i]);
    }
    for(int i = 0; i < this->controlPoints.size(); i++){
        std::cout << this->controlPoints[i] << std::endl;
    }
}

void PoligonalPathManager::updatePlayerPoint(Ogre::Vector3 playerPosition){
    if(playerPosition == controlPoints[playerNextPointIndex]){
        playerPointIndex = playerNextPointIndex;
        if(playerNextPointIndex == controlPoints.size() - 1){
            playerNextPointIndex = 0;
        }
        else {
            playerNextPointIndex += 1;
        }
    }
    //std::cout << "Player point = " << controlPoints[playerPointIndex] << std::endl;
}

void PoligonalPathManager::updateMonsterPoint(Ogre::Vector3 monsterPosition){
    if(monsterPosition == controlPoints[monsterNextPointIndex]){
        monsterPointIndex = monsterNextPointIndex;
        if(monsterNextPointIndex == controlPoints.size() - 1){
            monsterNextPointIndex = 0;
        }
        else {
            monsterNextPointIndex += 1;
        }
    }
    //std::cout << "Monster point = " << controlPoints[monsterPointIndex] << std::endl;
    //std::cout << "monster actual index = " << monsterPointIndex << std:: endl;
    //std::cout << "monster next index = " << monsterNextPointIndex << std:: endl;
}

void PoligonalPathManager::updateTangents(){
    playerLastTangent = playerCurrentTangent;
    playerCurrentTangent = controlPoints[playerNextPointIndex] - controlPoints[playerPointIndex];
    playerCurrentTangent.normalise();
    monsterLastTangent = monsterCurrentTangent;
    monsterCurrentTangent = controlPoints[monsterNextPointIndex] - controlPoints[monsterPointIndex];
    monsterCurrentTangent.normalise();
    //std::cout << "playerCurrentTangent = " << playerCurrentTangent << std::endl;
    //std::cout << "monsterCurrentTangent = " << monsterCurrentTangent << std::endl;
    //std::cout << "monsterLastTangent = " << monsterLastTangent << std::endl;
}

void PoligonalPathManager::setDebug(bool debug) {
    /*Ogre::SceneManager* sceneManager = Ogre::Root::getSingleton().getSceneManager("sceneManager");
       static bool firstTimeInit = true;

        if(firstTimeInit) {
            firstTimeInit = !firstTimeInit;
            Ogre::SceneNode* debugPoligonalPathManagerSceneNode = sceneManager->getRootSceneNode()->createChildSceneNode("debugPoligonalPathManagerSceneNode");

            for(const auto& point: controlPoints) {
                const double DEBUG_NODE_SCALE = 0.25;
                Ogre::Entity* entity = sceneManager->createEntity("sphere.mesh");
                entity->setMaterialName("Cycleshooter/Matheus");
                Ogre::SceneNode* sceneNode = debugPoligonalPathManagerSceneNode->createChildSceneNode();
                sceneNode->translate(point);
                sceneNode->scale(Ogre::Vector3(DEBUG_NODE_SCALE, DEBUG_NODE_SCALE, DEBUG_NODE_SCALE));
                sceneNode->attachObject(entity);
            }
        }

        sceneManager->getSceneNode("debugPathManagerSceneNode")->setVisible(debug);*/
}

PoligonalPathManager::~PoligonalPathManager() {
}

}
