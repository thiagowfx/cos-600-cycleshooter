#include "CollisionHandler.hpp"

namespace Cycleshooter {

CollisionHandler::CollisionHandler(Ogre::String collisionTexturePath):
    collisionTexture(0),
    collisionTexturePath(collisionTexturePath)
{
}

CollisionHandler::~CollisionHandler() {
    if(collisionTexture)
        delete collisionTexture;
}

void CollisionHandler::loadImages(){
    collisionTexture = new Ogre::Image;
    collisionTexture->load(collisionTexturePath,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    Ogre::uint32 imgW, imgH;
    imgW = collisionTexture->getWidth();
    imgH = collisionTexture->getHeight();
    std::cout<<"Testing some images functions." << std::endl;
    std::cout<<"Image Width in pixels ." << imgW<< std::endl;
    std::cout<<"Image Height in pixels ." << imgH<< std::endl;
    for (int i = 0;i < 400;i+=10){
        for (int j = 0; j<300; j+=10){
            Ogre::ColourValue pixel = collisionTexture->getColourAt(i,j,0);
            std::cout<<"Image Colour value at pixel (" << i<<","<<j<<") "<< pixel.r<<","<<pixel.g<< ","<<pixel.b<< std::endl;
        }
    }
    std::cout<<"Testing some images functions." << std::endl;
}

void CollisionHandler::loadTensor(){
    //Grabing images informations.
    int collisionWidth, circuitHeight;
    collisionWidth = static_cast<Ogre::uint32> (collisionTexture->getWidth());
    circuitHeight = static_cast<Ogre::uint32> (collisionTexture->getHeight());
    //Pixel color to both images.
    Ogre::ColourValue collisionPixel;
    //Reading (per rows) images for data.
    for(int w = 0; w <= circuitHeight; w++){
        for(int h = 0; h <= collisionWidth; h++){
            collisionPixel = collisionTexture->getColourAt(w,h,0);
        }
    }
}
}
