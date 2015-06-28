#ifndef _COLLISIONHANDLER_HPP_
#define _COLLISIONHANDLER_HPP_

//Class to deal with collisions and image loading.
#include <Ogre.h>
#include <fstream>
#include <iostream>
#include <utility>
#include <map>
#include <random>

namespace Cycleshooter {
class CollisionHandler {
    const Ogre::ColourValue BULLET_COLOR = Ogre::ColourValue(0.0f,1.0f,1.0f); // dentro
    const Ogre::ColourValue WATER_COLOR = Ogre::ColourValue(0.0f,0.0f,1.0f);
    const Ogre::ColourValue ROAD_COLOR = Ogre::ColourValue(0.5f,0.5f,0.5f); //Road
    const Ogre::ColourValue ROCK_COLOR = Ogre::ColourValue(0.0f,1.0f,0.0f); //Rock
    const Ogre::ColourValue GUAGMIRE_COLOR = Ogre::ColourValue(1.0f,0.0f,1.0f); //Guagmire
    const Ogre::ColourValue START_COLOR = Ogre::ColourValue(1.0f,1.0f,1.0f);

    Ogre::Image* collisionTexture; //Texture to specify terrain type.
    Ogre::String collisionTexturePath; //Path to circuit image.

    enum Colors{
        NONE_PIXEL,
        START_PIXEL,
        BULLET_PIXEL,
        ROAD_PIXEL,
        ROCK_PIXEL,
        GUAGMIRE_PIXEL,
        WATER_PIXEL,
    };
    int collisionMatrixWidth, collisionMatrixHeight;
    std::vector<std::vector<Colors> > collisionMatrix;
    std::vector<std::vector<std::pair<bool, Ogre::Vector3> > > bulletMatrix;

public:

    CollisionHandler(Ogre::String collisionTexturePath);
    virtual ~CollisionHandler();

    void loadImages(); //Load images from files.
    void loadTensor(); //Function to load the data structure from images.
    Colors getPixelEnumeration(int pixelWidth, int pixelHeight); //
    //Function to find the race starting point in the texture.
    std::pair<int,int> getStartPixel();
    std::vector<std::vector<Colors> > getCollisionMatrix();
    //Matrix dimensions getters.
    int getCollisionMatrixWidth() const;
    int getCollisionMatrixHeight() const;

    //Functions to add and remove bullets.
    void setBulletAt(int width, int height, bool exist, Ogre::Vector3 coord);

    //Testing functions
    void printMatrix(); //Testing funcition for collisionMatrix.
    void printBullets(); //Testing function for bulletMatrix.
    bool testMatrixDimension(); //Testing function to allocated matrix dimensions.
};

}

#endif
