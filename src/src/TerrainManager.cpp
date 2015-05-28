#include "TerrainManager.hpp"

namespace Cycleshooter {

TerrainManager::TerrainManager(Ogre::SceneManager* sceneManager, std::string collisionTexturePath):
    sceneManager(sceneManager),
    terrainWidth(0),
    terrainHeight(0),
    terrainWorldSizeHeight(0),
    terrainWorldSizeWidth(0),
    widthScale(0),
    heightScale(0),
    terrainTranslation(0,0,0)
{
    //Initializing collision handler.
    collisionHandler = new CollisionHandler(collisionTexturePath);
    collisionHandler->loadTensor();
}

TerrainManager::~TerrainManager() {
}

void TerrainManager::createTerrainLight(){
    sceneManager->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
    //Defines light direction.
    Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();
    //Creates the light pointing to terrain.
    Ogre::Light* light = sceneManager->createLight("TerrainLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
}

void TerrainManager::defineTerrainProperties(){
    //Defining quadrangular terrain.
    terrainWorldSizeHeight = 15000;
    terrainWorldSizeWidth = 15000;
    terrainWidth = 20;
    terrainHeight = 20;
}

void TerrainManager::createTerrain(){
    //Initializing a planar terrain structure.
    defineTerrainProperties();
    setCollisionTransformation();
    printCollisionTransformation();
    sampleCollisionTransformation();
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -100);
    Ogre::MeshManager::getSingleton().createPlane(
      "groundTerrain",
      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
      plane,
      terrainWorldSizeWidth, terrainWorldSizeHeight, terrainWidth,terrainHeight,
      true,
      1, 1,1,
      Ogre::Vector3::UNIT_Z);
    //Transforming planar mesh to treatable entity.
    Ogre::Entity* terrainEntity = sceneManager->createEntity("groundTerrain");
    //Adding entity to the scene.
    sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(terrainEntity);
    terrainEntity->setCastShadows(false);
    //Defines which texture will be used. 
    terrainEntity->setMaterialName("Examples/Ground");
}

void TerrainManager::setCollisionTransformation(){
    //Get datastructure dimensions.
    int collisionWidth = collisionHandler->getCollisionMatrixWidth();
    int collisionHeigth = collisionHandler->getCollisionMatrixHeight();
    heightScale = collisionHeigth/terrainWorldSizeHeight;
    widthScale = collisionWidth/terrainWorldSizeWidth;
    terrainTranslation.x = terrainWorldSizeWidth*0.5;
    terrainTranslation.y = 0;
    terrainTranslation.z = -terrainWorldSizeWidth*0.5;
}

void TerrainManager::printCollisionTransformation(){
    std::cout << " Translation Vector: " << terrainTranslation.x << "," << terrainTranslation.y << ","
              << terrainTranslation.z << std::endl;
    std::cout << "Width,Height Scale: " << widthScale << ","<< heightScale <<std::endl;
}

std::pair<int, int> TerrainManager::getCollisionCoordinates(Ogre::Vector3 point){
    std::pair<int, int> coord (0,0);
    Ogre::Real coordX = point.x + terrainTranslation.x;
    coordX = coordX*widthScale;
    Ogre::Real coordY = point.z + terrainTranslation.z;
    coordY = coordY*heightScale;
    coord.first = static_cast<int>(coordX);
    coord.second = static_cast<int>(coordY);
    return coord;
}

void TerrainManager::sampleCollisionTransformation(){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            Ogre::Vector3 rc(i*234.2,0,j*124.93);
            std::pair<int, int> coord = getCollisionCoordinates(rc);
            std::cout<< "Sampling Coordinates: "<< i<<","<<j<<" = " <<  coord.first << "," << coord.second << std::endl;
        }
    }
}
}
