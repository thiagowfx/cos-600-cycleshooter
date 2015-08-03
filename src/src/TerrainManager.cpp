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
    createTrees(100);
    createTerrainLake();
    createTerrainWall();

    //Defining terrain structures
    const int INITIAL_NUMBER_OF_BULLETS = ConfigManager::instance().getInt("TerrainManager.initial_number_of_bullets");
    generateBullets(INITIAL_NUMBER_OF_BULLETS);
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
    std::vector<Ogre::Vector3> sortedpoints;

    for(int i = 0; i< locations.size();i++){
        points.push_back(getWorldCoordinates(locations[i]));
    }

    LOG("Controll Points.");
    for(int i= 0; i < points.size();i++){
        std::cout<< points[i].x << " " << points[i].y << " " << points[i].z << std::endl;
    }

    auto sortControllPoints = [](std::vector<Ogre::Vector3>& pts)
                              -> std::vector<Ogre::Vector3>
    {
        std::vector<Ogre::Vector3> firstQuad;
        std::vector<Ogre::Vector3> secondQuad;
        std::vector<Ogre::Vector3> thirdQuad;
        std::vector<Ogre::Vector3> fourthQuad;

        // Separate the points in quadrants
        for(int i = 0; i < pts.size(); i++){
            if(pts.at(i).z >= 0  && pts.at(i).x >= 0)     firstQuad.push_back(pts.at(i));
            else if(pts.at(i).z < 0  && pts.at(i).x >= 0) secondQuad.push_back(pts.at(i));
            else if(pts.at(i).z < 0  && pts.at(i).x < 0)  thirdQuad.push_back(pts.at(i));
            else if(pts.at(i).z >= 0  && pts.at(i).x < 0) fourthQuad.push_back(pts.at(i));
        }

        // Sort the elements by they tangents and reverse them
        auto sortOgrePoints = [](const Ogre::Vector3 a, const Ogre::Vector3 b){
            return std::abs(a.z/a.x) < std::abs(b.z/b.x);
        };

        std::sort(firstQuad.begin(),firstQuad.end(),sortOgrePoints);
        std::sort(secondQuad.begin(),secondQuad.end(),sortOgrePoints);
        std::sort(thirdQuad.begin(),thirdQuad.end(),sortOgrePoints);
        std::sort(fourthQuad.begin(),fourthQuad.end(),sortOgrePoints);

        // Notice that we don't need reverse for the second and the fourth quadrant
        std::reverse(firstQuad.begin(),firstQuad.end());
        std::reverse(thirdQuad.begin(),thirdQuad.end());

        // Concatenate the vectors ordenaded
        pts.clear();
        pts.reserve(firstQuad.size()  +
                    secondQuad.size() +
                    thirdQuad.size()  +
                    fourthQuad.size());

        pts.insert(pts.end(), firstQuad.begin(), firstQuad.end());
        pts.insert(pts.end(), secondQuad.begin(), secondQuad.end());
        pts.insert(pts.end(), thirdQuad.begin(), thirdQuad.end());
        pts.insert(pts.end(), fourthQuad.begin(), fourthQuad.end());

        std::vector<Ogre::Vector3> filteredpts;

        std::cout << "Total Points: " << pts.size() << std::endl;
        std::cout << " Percentage : " << std::floor(0.2*(pts.size())) << std::endl;

        for(int i = 0; i < pts.size(); i = i + std::floor(0.01*(pts.size()))){
            filteredpts.push_back(pts.at(i));
        }

        return filteredpts;
    };

    sortedpoints = sortControllPoints(points);

    std::cout << std::endl <<"====== Printing sorted Ogre Control Points ======" << std::endl;
    for(int i = 0; i < sortedpoints.size(); i++){
        std::cout << sortedpoints.at(i).x <<" "
                  << sortedpoints.at(i).y <<" "
                  << sortedpoints.at(i).z << std::endl;
    }

    std::cout << "End of Sorted Ogre Control Points" << std::endl;
    return sortedpoints;
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

std::vector<int> TerrainManager::randomIdOfPoints(int maxvalue, int numOfPoints){
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
    std::vector<int> idx = randomIdOfPoints(controlpts.size(), numOfBullets);

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

void TerrainManager::createTrees(int numberOfTrees){

    std::vector<std::pair<int,int> > locations = collisionHandler->getGrassPoints();
    std::vector<Ogre::Vector3> points;

    for(int i = 0; i< locations.size();i++){
        points.push_back(getWorldCoordinates(locations[i]));
    }

    std::vector<int> idx = randomIdOfPoints(points.size(), numberOfTrees);

    /*for(int i = 0;i < idx.size();i++){
        Ogre::Entity* treeEntity = sceneManager->createEntity("treeEntity", "tree.mesh");
        Ogre::SceneNode* treeNode = sceneManager->getRootSceneNode()->createChildSceneNode("treeNode", points.at(idx.at(i)));
        treeNode->attachObject(treeEntity);
    }*/
}

void TerrainManager::resetBullets(){

}
}
