#ifndef _COLLISIONHANDLER_HPP_
#define _COLLISIONHANDLER_HPP_

//Class to deal with collisions and image loading.
#include <OgreImage.h>
#include <OgreColourValue.h>
#include <OgreString.h>
#include <OgreResourceGroupManager.h>
#include <OgreTextureManager.h>
#include <OgreDataStream.h>
#include <fstream>
#include <iostream>

namespace Cycleshooter {
class CollisionHandler {
    const int TERRAIN_SIZE = 512;
    const Ogre::ColourValue COL_WATER = Ogre::ColourValue (0.0f,0.0f,1.0f,1.0f);
    const Ogre::ColourValue COL_ROAD = Ogre::ColourValue(0.5f,0.5f,0.5f); //Road
    const Ogre::ColourValue COL_ROCK = Ogre::ColourValue(1.0f,1.0f,0.0f); //Rock
    const Ogre::ColourValue COL_GUAGMIRE = Ogre::ColourValue(1.0f,0.0f,1.0f); //Guagmire
    const Ogre::ColourValue COL_BULLET = Ogre::ColourValue(0.0f,1.0f,1.0f);

    Ogre::Image* collisionTexture; //Texture to specify terrain type.
    Ogre::String collisionTexturePath; //Path to circuit image.

    enum Textures{
        TEX_BULLET,
        TEX_WATER,
        TEX_ROAD,
        TEX_ROCK,
        TEX_GUAGMIRE,
    };

    std::vector<std::vector<int> > terrainMatrix;


public:

    CollisionHandler(Ogre::String collisionTexturePath);
    virtual ~CollisionHandler();
    void loadImages(); //Load images from files.
    void loadTensor(); //Function to load the data structure from images.

};

}

#endif
