#ifndef _COLLISIONHANDLER_HPP_
#define _COLLISIONHANDLER_HPP_

//Class to deal with collisions and image loading.
#include <Ogre.h>
#include <fstream>
#include <iostream>
#include <utility>
#include <map>

namespace Cycleshooter {
class CollisionHandler {
    const Ogre::ColourValue BULLET_COLOR = Ogre::ColourValue(0.0f,1.0f,1.0f); // dentro
    const Ogre::ColourValue WATER_COLOR = Ogre::ColourValue(0.0f,0.0f,1.0f);
    const Ogre::ColourValue ROAD_COLOR = Ogre::ColourValue(0.5f,0.5f,0.5f); //Road
    const Ogre::ColourValue ROCK_COLOR = Ogre::ColourValue(0.0f,1.0f,0.0f); //Rock
    const Ogre::ColourValue GUAGMIRE_COLOR = Ogre::ColourValue(1.0f,0.0f,1.0f); //Guagmire
    const Ogre::ColourValue STAR_COLOR = Ogre::ColourValue(1.0f,1.0f,1.0f);

    Ogre::Image* collisionTexture; //Texture to specify terrain type.
    Ogre::String collisionTexturePath; //Path to circuit image.

    enum Colors{
        BULLET_PIXEL,
        WATER_PIXEL,
        ROAD_PIXEL,
        ROCK_PIXEL,
        GUAGMIRE_PIXEL,
        NONE_PIXEL,
        START_PIXEL
    };

    std::vector<std::vector<Colors> > collisionMatrix;
    //std::map<Ogre::ColourValue, Colors> collisionMap;

public:

    CollisionHandler(Ogre::String collisionTexturePath);
    virtual ~CollisionHandler();

    void loadImages(); //Load images from files.
    void loadTensor(); //Function to load the data structure from images.
    void printMatrix(); //Testing funcition for collisionMatrix.
    Colors getPixelEnumeration(int pixelWidth, int pixelHeight); //
    Ogre::ColourValue getPixelColour(int pixelWidth, int pixelHeight);
    //Function to find the race starting point in the texture.
    std::pair<int,int> getStartPixel();
    std::vector<std::vector<Colors> > getCollisionMatrix();

};

}

#endif
