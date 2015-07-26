#ifndef _TERRAINMANAGER_HPP_
#define _TERRAINMANAGER_HPP_

#include <Ogre.h>
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include <CollisionHandler.hpp>
#include <OgreSceneManager.h>

namespace Cycleshooter {

class TerrainManager {
private:
    Ogre::SceneManager* const sceneManager;

    /**
     * Terrain properties
     *  #Ligth of terrain (Not be confused with the sun light)
     *  
     */
    Ogre::Light* terrainLight = NULL;
    Ogre::String terrainType = "Plane";
    Ogre::Real terrainWorldSizeHeight,terrainWorldSizeWidth;
    
    /**
     * 
     */
    Ogre::Real widthScale, heightScale;
    
    /**
     * 
     */
    Ogre::Vector3 terrainTranslation;
    
    /**
     * 
     */
    int terrainWidth, terrainHeight;
    
    /**
     * 
     */
    CollisionHandler* collisionHandler = NULL;

    /**
     *
     */
    void defineTerrainProperties();
    
    /**
     * 
     */
    void createTerrain();
    
    /**
     * 
     */
    //Defines translation and scale to collision coordinates(maps continous rectangles in  discrete rectangles).
    void setCollisionTransformation();

public:
    TerrainManager(Ogre::SceneManager* sceneManager, std::string collisionTexturePath);
    virtual ~TerrainManager();
    
    /**
     * 
     */
    std::pair<int,int> getCollisionCoordinates(Ogre::Vector3 point);//Transfors Ogre 3d point in data structure indexes.
    
    /**
     * 
     */
    std::pair<int,bool> getTerrainAt(Ogre::Vector3 coord); //Method to discover terrain property and bullet existence.
    
    /**
     * 
     */
    void generateBullets(int numOfBullets);//Random bullets Generator.
    
    /**
     * 
     */
    void renderBullets();

      /////////////////////
     // Debug functions //
    /////////////////////
    
    /**
     * 
     */
    void printCollisionTransformation();
    
    /**
     * 
     */
    void sampleCollisionTransformation();
};
}

#endif
