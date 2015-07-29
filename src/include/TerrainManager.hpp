#ifndef _TERRAINMANAGER_HPP_
#define _TERRAINMANAGER_HPP_

#include <Ogre.h>
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include <CollisionHandler.hpp>
#include <OgreSceneManager.h>
#include <Logging.hpp>
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
     * Scales between the terrain
     * and collision data structure.
     */
    Ogre::Real widthScale, heightScale;
    
    /**
     * Translation from terrain in Ogre dimensions
     * to collision dimensions.
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
     * Returns the data structure coordinates to
     * a given world point.
     */
    std::pair<int,int> getCollisionCoordinates(Ogre::Vector3 point);//Transfors Ogre 3d point in data structure indexes.
    
    /**
     * Returns the WorldCoordinates of
     * a given Collision Coordinate.
     */
    Ogre::Vector3 getWorldCoordinates(std::pair<int,int> collisionCoord);

    /**
     * Discover terrain type and bullet existence
     * by point-bounding box intersection in a given terrain point.
     */
    std::pair<int,bool> getTerrainAt(Ogre::Vector3 coord);

    /**
     * Discover terrain type and bullet existence by
     * line-bounding box intersection in a given terrain point.
     */
    std::pair<int,bool> getTerrainAt(Ogre::Vector3 coord, Ogre::Vector3 lastCoord);

    /**
     * Defines if a line segment delimeted by p1 and p2 intersects
     * the bounding box bBox.
     */
    bool calculateSLBIntersection(Ogre::Vector3 p1,Ogre::Vector3 p2, Ogre::AxisAlignedBox bBox);

    /**
     * Obtain circuits controll points in world coordinates.
     */
    std::vector<Ogre::Vector3> obtainCircuitControllPoints();
    /**
     * 
     */

    std::vector<std::pair<int,int> > calculateBulletSurroundings(Ogre::Vector3 center,Ogre::AxisAlignedBox boundingBox);

    std::vector<std::pair<int,int> > calculateBulletSurroundings(Ogre::Vector3 center);

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
