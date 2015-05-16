#include "CollisionHandler.hpp"

namespace Cycleshooter {

CollisionHandler::CollisionHandler(Ogre::String circuitTexturePath, Ogre::String bulletsTexturePath):
    circuitTexture(0),
    bulletsTexture(0),
    circuitTexturePath(circuitTexturePath),
    bulletsTexturePath(bulletsTexturePath)
{
}

CollisionHandler::~CollisionHandler() {
    if(circuitTexture)
        delete circuitTexture;
    if(bulletsTexture)
        delete bulletsTexture;
}

void CollisionHandler::loadImages(){
    circuitTexture = new Ogre::Image;
    bulletsTexture = new Ogre::Image;
    circuitTexture->load(circuitTexturePath);
    bulletsTexture->load(bulletsTexturePath);
}

}
