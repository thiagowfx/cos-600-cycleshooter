#ifndef _COLLISIONHANDLER_HPP_
#define _COLLISIONHANDLER_HPP_

#include <Ogre.h>
#include <fstream>
#include <iostream>
#include <utility>
#include <random>
#include <BulletElement.hpp>
#include "Logging.hpp"

namespace Cycleshooter {
/**
 * List of colors present in the game scene.
 */
enum Colors{
    NONE_PIXEL,
    START_PIXEL,
    GRASS_PIXEL,
    BULLET_PIXEL,
    ROAD_PIXEL,
    ROCK_PIXEL,
    SAND_PIXEL,
    ICE_PIXEL,
    PATH_PIXEL,
    WATER_PIXEL
};
class CollisionHandler {
    /**
     * RGB value associated with the position of the bullet.
     */
    const Ogre::ColourValue BULLET_COLOR = Ogre::ColourValue(1.0f,0.5f,0.5f);

    /**
     * RGB value associated with the lake on the game.
     */
    Ogre::ColourValue GRASS_COLOR = Ogre::ColourValue(setOgreScale(0), setOgreScale(0), setOgreScale(0));
    
    /**
     * RGB value associated with the default road on the game.
     */
    const Ogre::ColourValue ROAD_COLOR = Ogre::ColourValue(setOgreScale(255), setOgreScale(0), setOgreScale(255));

    /**
     * RGB value associated with the Rocks on the road.
     */
    const Ogre::ColourValue ROCK_COLOR = Ogre::ColourValue(setOgreScale(0), setOgreScale(0), setOgreScale(255));

    /**
     * RGB value associated with the Sand on the road.
     */
    const Ogre::ColourValue SAND_COLOR = Ogre::ColourValue(setOgreScale(255), setOgreScale(255), setOgreScale(0));

    /**
     * RGB value associated with the Ice on the road.
     */
    const Ogre::ColourValue ICE_COLOR = Ogre::ColourValue(setOgreScale(0), setOgreScale(255), setOgreScale(0));

    /**
     * RGB value associated with the Start Point of the player.
     */
    const Ogre::ColourValue START_COLOR = Ogre::ColourValue(setOgreScale(255), setOgreScale(255), setOgreScale(255));

    /**
     * 
     */
     const Ogre::ColourValue PATH_COLOR = Ogre::ColourValue(setOgreScale(127), setOgreScale(127), setOgreScale(0));

    /**
     * @brief collisionTexture
     */
    Ogre::Image* collisionTexture;

    /**
     * Path to the circuit image
     */
    Ogre::String collisionTexturePath;

    /**
     * 
     */
    int bulletCount;



    /**
     * Width and Height of the collision Matrix.
     */
    int collisionMatrixWidth, collisionMatrixHeight;

    /**
     * Assigne a region to each pixel of the image.
     */
    std::vector<std::vector<Colors> > collisionMatrix;

    /**
     * Matrix with the bullets information.
     */
    std::vector<std::vector<std::pair<bool, BulletElement> > > bulletMatrix;

    /**
     *
     */
    double setOgreScale(int x);


public:

    CollisionHandler(Ogre::String collisionTexturePath);
    virtual ~CollisionHandler();

    /**
     * Read an image file to an internal structure.
     */
    void loadImages();

    /**
     *  Function to get the information about each pixel of the image and load
     * in the data structure.
     */
    void loadTensor();

    /**
     * 
     */
    Colors getPixelEnumeration(int pixelWidth, int pixelHeight); //

    /**
      * Function to find the race starting point in the structure.
      */
    std::pair<int,int> getStartPixel();

    /**
     * Return the Collision Matrix (vector of vectors)
     */
    std::vector<std::vector<Colors> > getCollisionMatrix();

    /**
     * Returns the Collision Matrix Width.  
     */
    int getCollisionMatrixWidth() const;

    /**
     * Returns the Collision Matrix Height.
     */
    int getCollisionMatrixHeight() const;

    /**
     *  Function that discover if a bullet exists
     * in a data structure's element.
     */
    bool existBulletAt(int pixelWidth, int pixelHeight);

    /**
     *  Function that discover bullet's scene node name
     * and center.
     */
    std::pair<Ogre::Vector3,Ogre::String> getBulletPropertiesAt(int pixelWidth, int pixelHeight);

    /**
     *  Function that obtains all path control points
     * in the CollisionMatrix.
     */
    std::vector<std::pair<int,int> > getPathControllPoints();
    
    /**
     * Functions to add bullets in bullet's Matrix.
     */
    void insertBulletAt(int width, int height, bool exist, Ogre::Vector3 coord);
    
    /**
     * Return bullets scenenode names and centers.
     */
    std::pair<std::vector<Ogre::String> ,std::vector<Ogre::Vector3> > getBulletsForRender();

    /**
     * Function to dis/activate bullets.
     */
    void setBulletState(int width, int height,bool state);

    /**
     * Function to adapt bullet structure based on bullet rendering dimensions.
     */

    void compensateBulletRender(std::vector<std::pair<int,int> > coords);
      /////////////////////
     // Debug functions //
    /////////////////////
    
    /**
     * Check if the Collision Matrix is loaded correctly.
     */
    void printMatrix(); 

    /**
     * Check if the Bullet Matrix is correct.
     */
    void printBullets();

    /**
     * Testing function to allocated matrix dimensions.
     */
    bool testMatrixDimension();
};

}

#endif
