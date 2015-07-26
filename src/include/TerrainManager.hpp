#ifndef _TERRAINMANAGER_HPP_
#define _TERRAINMANAGER_HPP_

#include <Ogre.h>
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include <CollisionHandler.hpp>
#include <OgreSceneManager.h>

namespace Cycleshooter {

class TerrainManager {
    Ogre::SceneManager* const sceneManager;

    // customizable settings
    Ogre::Light* terrainLight;//Specific Ligth to terrain.

    //Terrain Variables.
    Ogre::String terrainType = "Plane";
    Ogre::Real terrainWorldSizeHeight,terrainWorldSizeWidth;
    //Transformation properties.
    Ogre::Real widthScale, heightScale;
    Ogre::Vector3 terrainTranslation;
    //Plane vertex dimensions.
    int terrainWidth, terrainHeight;
    CollisionHandler* collisionHandler;

public:
    TerrainManager(Ogre::SceneManager* sceneManager, std::string collisionTexturePath);
    virtual ~TerrainManager();
    void createTerrainLight();
    void defineTerrainProperties();
    void createTerrain();
    //Defines translation and scale to collision coordinates(maps continous rectangles in  discrete rectangles).
    void setCollisionTransformation();
    std::pair<int,int> getCollisionCoordinates(Ogre::Vector3 point);//Transfors Ogre 3d point in data structure indexes.
    std::pair<int,bool> getTerrainAt(Ogre::Vector3 coord); //Method to discover terrain property and bullet existence.
    std::vector<std::pair<int,int> > calculateBulletSurroundings(Ogre::Vector3 center, Ogre::Real radius);

    void generateBullets(int numOfBullets);//Random bullets Generator.
    void renderBullets();

    //Test Functions.
    void printCollisionTransformation();
    void sampleCollisionTransformation();
};

}

#endif
