#ifndef _CollisionHandler_HPP_
#define _CollisionHandler_HPP_

//Class to deal with collisions and image loading.
#include <OgreImage.h>
#include <OgreString.h>
#include <OgreResourceGroupManager.h>
#include <OgreTextureManager.h>
#include <OgreDataStream.h>
#include <fstream>

namespace Cycleshooter {
class CollisionHandler {

private:

    //Texture to specify terrain type.
    Ogre::Image* circuitTexture;
    //Texture to allocate bullets.
    Ogre::Image* bulletsTexture;
    //Path to circuit image.
    Ogre::String circuitTexturePath;
    //Path to bullets image.
    Ogre::String bulletsTexturePath;

public:

    CollisionHandler(Ogre::String circuitTexturePath, Ogre::String bulletsTexturePath);
    virtual ~CollisionHandler();
    //Load images from files.
    void loadImages();

};

}

#endif
