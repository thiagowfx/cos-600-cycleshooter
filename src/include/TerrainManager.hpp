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
    Ogre::Real terrainWorldSize;
    int terrainSize;

    collisionHandler* collisionHandler;
public:
    TerrainManager(Ogre::SceneManager* sceneManager, std::string collisionTexturePath);
    virtual ~TerrainManager();
    void createTerrainLight();
    void createTerrain();//Test Function to create a planar terrain.
};

}

#endif
