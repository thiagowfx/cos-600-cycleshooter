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
    terrainWorldSizeWidth = 20000;
    terrainWidth = 1;
    terrainHeight = 1;
}

void TerrainManager::createTerrain(){
    //Initializing a planar terrain structure.
    defineTerrainProperties();
    setCollisionTransformation();
    printCollisionTransformation();
    //sampleCollisionTransformation();
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
    generateBullets(100);
}

void TerrainManager::setCollisionTransformation(){
    //Get datastructure dimensions.
    int collisionWidth = collisionHandler->getCollisionMatrixWidth(); 
    int collisionHeigth = collisionHandler->getCollisionMatrixHeight();
    collisionHeigth -= 1;
    collisionWidth -= 1;
    std::cout << "Collision Lengths "<<collisionWidth << "," << collisionHeigth << std::endl;
    heightScale = collisionHeigth/terrainWorldSizeHeight;
    widthScale = collisionWidth/terrainWorldSizeWidth;
    terrainTranslation.x = terrainWorldSizeWidth*0.5;
    terrainTranslation.y = 0;
    terrainTranslation.z = terrainWorldSizeHeight*0.5;
}

void TerrainManager::printCollisionTransformation(){
    std::cout << " Translation Vector: " << terrainTranslation.x << "," << terrainTranslation.y << ","
              << terrainTranslation.z << std::endl;
    std::cout << "Width,Height Scale: " << widthScale << ","<< heightScale <<std::endl;
}

std::pair<int, int> TerrainManager::getCollisionCoordinates(Ogre::Vector3 point){
    std::pair<int, int> coord (0,0);
    Ogre::Real coordX = terrainTranslation.x - point.x;
    coordX = coordX*widthScale;
    Ogre::Real coordY = terrainTranslation.z - point.z;
    coordY = coordY*heightScale;
    coord.first = static_cast<int>(coordX);
    coord.second = static_cast<int>(coordY);
    return coord;
}

int TerrainManager::getTerrainAt(Ogre::Vector3 coord){
    std::pair<int,int> collisionCoord = getCollisionCoordinates(coord);
    if(coord.x> terrainWorldSizeWidth*0.5 || coord.y > terrainWorldSizeHeight*0.5)
        return 0;
    return collisionHandler->getPixelEnumeration(collisionCoord.first,collisionCoord.second);
}

void TerrainManager::generateBullets(int numOfBullets){
    std::default_random_engine randomGenerator;
    Ogre::Real wlimit = terrainWorldSizeWidth*0.5;
    Ogre::Real hLimit = terrainWorldSizeHeight*0.5;
    std::uniform_real_distribution<float> randomWidthDistribution (-wlimit,wlimit);
    std::uniform_real_distribution<float> randomHeightDistribution (-hLimit,hLimit);
    float randomWidth, randomHeight = 0.0f;
    int test = 0;
    std::cout << "Testing Random coordinates." << std::endl;
    for(int i= 0;i<numOfBullets;i++){
        randomWidth = randomWidthDistribution(randomGenerator);
        randomHeight = randomHeightDistribution(randomGenerator);
        Ogre::Vector3 coord(randomWidth,0,randomHeight);
        std::pair<int,int> location = getCollisionCoordinates(coord);
        test++;
        collisionHandler->setBulletAt(location.first,location.second,true,coord);
        std::cout << "Width = " << randomWidth <<" Height = "<<randomHeight<<" i = "<<i <<std::endl;
    }
    std::cout<<"Testing = " << test <<std::endl;
    collisionHandler->printBullets();
}

void TerrainManager::sampleCollisionTransformation(){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            Ogre::Vector3 rc(i*234.2,0,j*124.93);
            std::pair<int, int> coord = getCollisionCoordinates(rc);
            std::cout << "Pixel Enumeration" << collisionHandler->getPixelEnumeration(coord.first,coord.second)<<std::endl;
            std::cout << "Sampling Coordinates: "<< i<<","<<j<<" = " <<  coord.first << "," << coord.second << std::endl;
            std::cout << "Tested Coordinates: " << Ogre::StringConverter::toString(rc) << std::endl;
        }
    }
}
}
