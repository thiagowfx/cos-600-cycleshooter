#ifndef _COLLISIONHANDLER_HPP_
#define _COLLISIONHANDLER_HPP_

#include <Ogre.h>
#include <fstream>
#include <iostream>
#include <utility>
#include <random>
#include <BulletElement.hpp>

namespace Cycleshooter {
class CollisionHandler {
    /**
     * RGB value associated with the position of the bullet.
     */
    const Ogre::ColourValue BULLET_COLOR = Ogre::ColourValue(0.0f,1.0f,1.0f);

    /**
     * RGB value associated with the lake on the game.
     */
    const Ogre::ColourValue WATER_COLOR = Ogre::ColourValue(0.247f,0.843f,0.898f);

    /**
     * RGB value associated with the lake on the game.
     */
    const Ogre::ColourValue GRASS_COLOR = Ogre::ColourValue(0.0f,0.0f,0.0f);
    
    /**
     * RGB value associated with the default road on the game.
     */
    const Ogre::ColourValue ROAD_COLOR = Ogre::ColourValue(1.0f,0.0f,1.0f);

    /**
     * RGB value associated with the Rocks on the road.
     */
    const Ogre::ColourValue ROCK_COLOR = Ogre::ColourValue(0.243f,0.259f,0.243f);

    /**
     * RGB value associated with the Sand on the road.
     */
    const Ogre::ColourValue SAND_COLOR = Ogre::ColourValue(0.863f,0.835f,0.129f);

    /**
     * RGB value associated with the Ice on the road.
     */
    const Ogre::ColourValue ICE_COLOR = Ogre::ColourValue(0.0f,0.0f,1.0f);

    /**
     * RGB value associated with the Start Point of the player.
     */
    const Ogre::ColourValue START_COLOR = Ogre::ColourValue(1.0f,1.0f,1.0f);

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
        WATER_PIXEL
    };

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
     *  Function that discover if a bullet exists and also 
     * the scene node name related to it.
     */
    std::pair<bool, Ogre::String> getBulletNameAt(int pixelWidth, int pixelHeight);
    
    /**
     * Functions to add bullets in bullet's Matrix.
     */
    void insertBulletAt(int width, int height, bool exist, Ogre::Vector3 coord);

    
    /**
     * Function to dis/activate bullets (Useful to not pick infinite bullets).
     */
    void toogleBulletState(int width, int height);

    
    /**
     * Return bullets scenenode names and centers.
     */
    std::pair<std::vector<Ogre::String> ,std::vector<Ogre::Vector3> > getBulletsForRender();

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
