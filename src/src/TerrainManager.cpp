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
    terrainEntity->setMaterialName("Cycleshooter/Ground");
    generateBullets(13);
    renderBullets();
}

void TerrainManager::setCollisionTransformation(){
    //Get data structure dimensions.
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
    Ogre::Real coordX = terrainTranslation.x - point.x;
    coordX = coordX*widthScale;
    Ogre::Real coordY = terrainTranslation.z - point.z;
    coordY = coordY*heightScale;
    return std::pair<int,int>(coordX,coordY);
}

std::pair<int, bool> TerrainManager::getTerrainAt(Ogre::Vector3 coord){
    std::pair<int,int> collisionCoord = getCollisionCoordinates(coord);
    //Getting terrain property.
    std::pair<int,bool> terrainAt = std::make_pair(0,false);
    //Prevents segfault. In release, the clause must not be true anytime.
    if(coord.x> terrainWorldSizeWidth*0.5 || coord.y > terrainWorldSizeHeight*0.5){
        return std::make_pair(-1,false);
    }
    terrainAt.first = collisionHandler->getPixelEnumeration(collisionCoord.first,collisionCoord.second);
    //Discover if a bullet exists in the terrain point.
    //Ogre::Real rad = sceneManager->getSceneNode("bulletSceneNode0")->getAttachedObject("bulletSceneNode0")->getBoundingRadius();
    std::pair<bool ,Ogre::String> bulletProperties = collisionHandler->getBulletNameAt(collisionCoord.first,collisionCoord.second);
    //terrainAt.second = sceneManager->getSceneNode(bulletName)->getAttachedObject(bulletName)->getBoundingBox().contains(coord);
    if(bulletProperties.first){
        Ogre::LogManager::getSingletonPtr()->logMessage("--> TerrainManager: Exist Bullet Here! <--");
        Ogre::AxisAlignedBox bulletBox = sceneManager->getSceneNode(bulletProperties.second)->getAttachedObject(bulletProperties.second)->getWorldBoundingBox(true);
        LOG("Bounding Box have been obtained.");
        std::cout << bulletBox.volume() << std::endl;
        std::cout << bulletBox.getCenter().x<<" "<<bulletBox.getCenter().y<<" "<<bulletBox.getCenter().z<<std::endl;
        std::cout << coord.x<<" "<<coord.y<<" "<<coord.z<<std::endl;
        terrainAt.second = sceneManager->getSceneNode(bulletProperties.second)->getAttachedObject(bulletProperties.second)->getWorldBoundingBox().intersects(coord);
        std::cout << sceneManager->getSceneNode(bulletProperties.second)->getAttachedObject(bulletProperties.second)->getBoundingBox().intersects(coord)<<std::endl;
        if(terrainAt.second){
            Ogre::Real rad = sceneManager->getSceneNode(bulletProperties.second)->getAttachedObject(bulletProperties.second)->getBoundingRadius();
            sceneManager->getSceneNode(bulletProperties.second)->getAttachedObject(bulletProperties.second)->setVisible(false);
            std::vector<std::pair<int,int> > coords = calculateBulletSurroundings(bulletBox);
            for(int i = 0; i < coords.size();i++){
                collisionHandler->setBulletState(coords[i].first,coords[i].second,false);
            }
        }
    }

    //std::cout<< "terrainAt" << terrainAt.first << " " <<terrainAt.second<<std::endl;
    return terrainAt;
}

std::vector<std::pair<int, int> > TerrainManager::calculateBulletSurroundings(Ogre::AxisAlignedBox boundingBox){
    //Finding bullet entities bounding limits.
    //And Calculates transfomations to be passed to collision handler.
    std::pair<int,int> coord0 = getCollisionCoordinates(boundingBox.getCenter());
    std::pair<int,int> coord1 = getCollisionCoordinates(boundingBox.getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_BOTTOM));
    std::pair<int,int> coord2 = getCollisionCoordinates(boundingBox.getCorner(Ogre::AxisAlignedBox::NEAR_RIGHT_BOTTOM));
    std::pair<int,int> coord3 = getCollisionCoordinates(boundingBox.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM));
    std::pair<int,int> coord4 = getCollisionCoordinates(boundingBox.getCorner(Ogre::AxisAlignedBox::FAR_RIGHT_BOTTOM));
    std::vector<std::pair<int,int> > coords;
    //Adding center at first position.
    coords.push_back(coord0);
    coords.push_back(coord1);coords.push_back(coord2);coords.push_back(coord3);coords.push_back(coord4);
    std::cout << "Center at "<<coords[0].first << ","<< coords[0].second<< std::endl;
    for(int i = 1; i< coords.size();i++){
        std::cout << "Compensation at "<<coords[i].first << ","<< coords[i].second<< std::endl;
    }
    return coords;
}

void TerrainManager::generateBullets(int numOfBullets){
    Ogre::LogManager::getSingletonPtr()->logMessage("--> TerrainManager: Generatig Random Values <--");
    std::default_random_engine randomGenerator;
    //Defining limits on terrain where bullets will appear.
    Ogre::Real wlimit = terrainWorldSizeWidth*0.125;
    Ogre::Real hLimit = terrainWorldSizeHeight*0.125;
    //Initializing pseudo-random generators.
    std::uniform_real_distribution<float> randomWidthDistribution (-wlimit,wlimit);
    std::uniform_real_distribution<float> randomHeightDistribution (-hLimit,hLimit);
    float randomWidth, randomHeight = 0.0f;
    //TODO :  Test variable that must be excluded to release.
    //int test = 0;
    //std::cout << "Testing Random coordinates." << std::endl;
    //Generating random values.
    for(int i= 0;i<numOfBullets;i++){
        randomWidth = randomWidthDistribution(randomGenerator);
        randomHeight = randomHeightDistribution(randomGenerator);
        Ogre::Vector3 coord(randomWidth,0,randomHeight);
        std::pair<int,int> location = getCollisionCoordinates(coord);
        //test++;
        //Inserting random bullets in collision handler.
        //TODO: Correct bugs in the bullets number.
        //Adding bullet to the dataStructure in CollisionHandler.
        collisionHandler->insertBulletAt(location.first,location.second,true,coord);
        //std::cout << "Width = " << randomWidth <<" Height = "<<randomHeight<<" i = "<<i <<std::endl;
    }
    //std::cout<<"Testing = " << test <<std::endl;
    Ogre::LogManager::getSingletonPtr()->logMessage("--> TerrainManager: Rendering Bullets <--");
}

void TerrainManager::renderBullets(){
    Ogre::LogManager::getSingletonPtr()->logMessage("--> TerrainManager: Preparing Bullets <--");
    std::pair<std::vector<Ogre::String> , std::vector<Ogre::Vector3> > renderSettings = collisionHandler-> getBulletsForRender();
    for(int i = 0;i < renderSettings.first.size();i++){
        Ogre::Entity* bulletEntity = sceneManager->createEntity(renderSettings.first[i], "bullet.mesh");
        bulletEntity->setMaterialName("Cycleshooter/BulletShell");
        Ogre::LogManager::getSingletonPtr()->logMessage("--> TerrainManger: Rendering Bullet <--");
        Ogre::SceneNode* bulletNode = sceneManager->getRootSceneNode()->createChildSceneNode(renderSettings.first[i], renderSettings.second[i]);
        Ogre::LogManager::getSingletonPtr()->logMessage(renderSettings.first[i]);
        bulletNode->attachObject(bulletEntity);
        //Compensates sizes defined by bulletBoundingBox.
        collisionHandler->compensateBulletRender(calculateBulletSurroundings(bulletNode->getAttachedObject(renderSettings.first[i])->getWorldBoundingBox(true)));
    }
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
