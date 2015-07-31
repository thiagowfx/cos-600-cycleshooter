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

    // Creating the terrain
    createTerrain();
}

TerrainManager::~TerrainManager() {
    delete collisionHandler;
}

void TerrainManager::defineTerrainProperties(){
    //Defining quadrangular terrain.
    terrainWorldSizeHeight  = 25000;
    terrainWorldSizeWidth   = 25000;
    terrainWidth    = 1;
    terrainHeight   = 1;
    GRASS_HEIGHT = 200;
    GRASS_WIDTH = 200;

}

void TerrainManager::createTerrain(){
    //Initializing a planar terrain structure.
    defineTerrainProperties();
    setCollisionTransformation();
    printCollisionTransformation();

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
    Ogre::Entity* terrainEntity = sceneManager->createEntity("Ground","groundTerrain");
    terrainEntity->setMaterialName("Cycleshooter/Ground");

    //Adding entity to the scene.
    terrainEntity->setCastShadows(false);
    sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(terrainEntity);

    // Creating terrain structures
    createTerrainLake();
    createTerrainWall();

    //Defining terrain structures
    generateBullets(13);
    renderBullets();
    obtainCircuitControllPoints();
}

void TerrainManager::setCollisionTransformation(){
    //Get data structure dimensions.
    int collisionWidth = collisionHandler->getCollisionMatrixWidth(); 
    int collisionHeigth = collisionHandler->getCollisionMatrixHeight();
    collisionHeigth -= 1;
    collisionWidth -= 1;

    heightScale = collisionHeigth / terrainWorldSizeHeight;
    widthScale = collisionWidth /terrainWorldSizeWidth;
    
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

Ogre::Vector3 TerrainManager::getWorldCoordinates(std::pair<int, int> collisionCoord){
    Ogre::Real coordX = collisionCoord.first*1.0;
    Ogre::Real coordZ = collisionCoord.second*1.0;
    coordX =  coordX * terrainWorldSizeWidth/collisionHandler->getCollisionMatrixWidth();
    coordZ = coordZ * terrainWorldSizeHeight/collisionHandler->getCollisionMatrixHeight();
    coordX = terrainWorldSizeWidth*0.5 - coordX;
    coordZ = terrainWorldSizeHeight*0.5 - coordZ;
    return Ogre::Vector3(coordX,0,coordZ);
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

    //terrainAt.second = sceneManager->getSceneNode(bulletName)->getAttachedObject(bulletName)->getBoundingBox().contains(coord);
    if(collisionHandler->existBulletAt(collisionCoord.first,collisionCoord.second)){
        std::pair<Ogre::Vector3 ,Ogre::String> bulletProperties = collisionHandler->getBulletPropertiesAt(collisionCoord.first,collisionCoord.second);
        Ogre::LogManager::getSingletonPtr()->logMessage("--> TerrainManager: Exist Bullet Here! <--");
        Ogre::AxisAlignedBox bulletBox = sceneManager->getSceneNode(bulletProperties.second)->getAttachedObject(bulletProperties.second)->getWorldBoundingBox(true);
        LOG("Bounding Box have been obtained.");
        std::cout << bulletBox.volume() << std::endl;
        std::cout << bulletBox.getCenter().x<<" "<<bulletBox.getCenter().y<<" "<<bulletBox.getCenter().z<<std::endl;
        std::cout << coord.x<<" "<<coord.y<<" "<<coord.z<<std::endl;
        terrainAt.second = sceneManager->getSceneNode(bulletProperties.second)->getAttachedObject(bulletProperties.second)->getWorldBoundingBox().intersects(coord);
        std::cout << sceneManager->getSceneNode(bulletProperties.second)->getAttachedObject(bulletProperties.second)->getBoundingBox().intersects(coord)<<std::endl;
        if(terrainAt.second){
            sceneManager->getSceneNode(bulletProperties.second)->getAttachedObject(bulletProperties.second)->setVisible(false);
            std::vector<std::pair<int,int> > coords = calculateBulletSurroundings(bulletProperties.first);
            for(int i = 0; i < coords.size();i++){
                collisionHandler->setBulletState(coords[i].first,coords[i].second,false);
            }
        }
    }

    return terrainAt;
}

std::vector<Ogre::Vector3> TerrainManager::obtainCircuitControllPoints(){
    std::vector<std::pair<int,int> > locations = collisionHandler->getPathControllPoints();
    std::vector<Ogre::Vector3> points;
    for(int i = 0; i< locations.size();i++){
        points.push_back(getWorldCoordinates(locations[i]));
    }
    LOG("Controll Points.");
    for(int i= 0; i < points.size();i++){
        std::cout<<points[i].x<<" "<<points[i].y<<" "<<points[i].z<<std::endl;
    }
    return points;
}

std::vector<std::pair<int, int> > TerrainManager::calculateBulletSurroundings(Ogre::Vector3 center,Ogre::AxisAlignedBox boundingBox){
    //Finding bullet entities bounding limits.
    //And Calculates transfomations to be passed to collision handler.
    std::pair<int,int> coord0 = getCollisionCoordinates(center);
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

std::vector<std::pair<int, int> > TerrainManager::calculateBulletSurroundings(Ogre::Vector3 center){
    std::pair<int,int> coord0 = getCollisionCoordinates(center);
    std::vector<std::pair<int,int> > coords;
    coords.push_back(coord0);
    for(int i = -5;i<6;i++){
        for(int j = -5;j<6;j++){
            coords.push_back(std::pair<int,int> (coord0.first+i,coord0.second+j));
        }
    }
    return coords;
}

std::vector<int> TerrainManager::randomIdOfBullets(int maxvalue, int numOfPoints){
    std::srand(std::time(NULL));
    std::vector<int> IDList;

    auto checkInList = [](std::vector<int> &list, int value) -> bool{
        for (int i = 0; i < list.size(); ++i){
            if (list.at(i) == value)
                return true;
        }
        return false;
    };

    while (IDList.size() < numOfPoints) {
        int num = rand() % maxvalue;

        if (!checkInList(IDList, num)){
            IDList.push_back(num);
        }
    }

    return IDList;
}

void TerrainManager::generateBullets(int numOfBullets){
    Ogre::LogManager::getSingletonPtr()->logMessage("--> TerrainManager: Generatig Random Values <--");
    std::vector<Ogre::Vector3> controlpts = obtainCircuitControllPoints();
    std::vector<int> idx = randomIdOfBullets(controlpts.size(), numOfBullets);

    //Generating random values.
    for(int i = 0; i < numOfBullets; i++){
        std::cout << "Random ID = " << idx[i] << std::endl << std::endl << std::endl;
        Ogre::Vector3 coord(controlpts.at(idx[i]));
        std::pair<int,int> location = getCollisionCoordinates(coord);
        
        //Adding bullet to the dataStructure in CollisionHandler.
        collisionHandler->insertBulletAt(location.first,location.second,true,coord);
    }
}

void TerrainManager::renderBullets(){
    Ogre::LogManager::getSingletonPtr()->logMessage("--> TerrainManager: Preparing Bullets <--");
    std::pair<std::vector<Ogre::String> , std::vector<Ogre::Vector3> > renderSettings = collisionHandler-> getBulletsForRender();

    for(int i = 0;i < renderSettings.first.size();i++){
        Ogre::Entity* bulletEntity = sceneManager->createEntity(renderSettings.first[i], "bullet.mesh");
        bulletEntity->setMaterialName("Cycleshooter/BulletShell");
        Ogre::SceneNode* bulletNode = sceneManager->getRootSceneNode()->createChildSceneNode(renderSettings.first[i], renderSettings.second[i]);
        bulletNode->attachObject(bulletEntity);

        //Compensates sizes defined by bulletBoundingBox.
        collisionHandler->compensateBulletRender(calculateBulletSurroundings(renderSettings.second[i]));
    }
}

void TerrainManager::createTerrainLake(){

}

void TerrainManager::createTerrainWall(){

}
}
