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
    circuitTexture->load(circuitTexturePath,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    bulletsTexture->load(bulletsTexturePath,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    Ogre::uint32 imgW, imgH;
    imgW = circuitTexture->getWidth();
    imgH = circuitTexture->getHeight();
    std::cout<<"Testing some images functions." << std::endl;
    std::cout<<"Image Width in pixels ." << imgW<< std::endl;
    std::cout<<"Image Height in pixels ." << imgH<< std::endl;
    for (int i = 0;i < 400;i+=10){
        for (int j = 0; j<300; j+=10){
            Ogre::ColourValue pixel = circuitTexture->getColourAt(i,j,0);
            std::cout<<"Image Colour value at pixel (" << i<<","<<j<<") "<< pixel.r<<","<<pixel.g<< ","<<pixel.b<< std::endl;
        }
    }
    std::cout<<"Testing some images functions." << std::endl;
}

}
