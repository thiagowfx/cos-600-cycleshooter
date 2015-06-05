#ifndef _TERRAINMANAGER_HPP_
#define _TERRAINMANAGER_HPP_

#include <Ogre.h>
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include <CollisionHandler.hpp>

namespace Cycleshooter {

class TerrainManager {
    Ogre::SceneManager* const sceneManager;

    // customizable settings
    Ogre::Light* terrainLight;//Specific Ligth to terrain.

    //Terrain Variables.
    Ogre::String terrainType = "Plane";
    Ogre::Real widthScale, heightScale,terrainWorldSizeHeight,terrainWorldSizeWidth;
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
    void setCollisionTransformation();//Defines translation and scale to collision coordinates.
    std::pair<int,int> getCollisionCoordinates(Ogre::Vector3 point);
    int getTerrainAt(Ogre::Vector3 coord);

    //Test Functions.
    void printCollisionTransformation();
    void sampleCollisionTransformation();
};

}

#endif
