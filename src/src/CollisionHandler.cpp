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
    /*std::cout<<"Testing some images functions." << std::endl;
    std::cout<<"Image Width in pixels ." << imgW<< std::endl;
    std::cout<<"Image Height in pixels ." << imgH<< std::endl;
    for (int i = 0;i < 400;i+=10){
        for (int j = 0; j<300; j+=10){
            Ogre::ColourValue pixel = collisionTexture->getColourAt(i,j,0);
            std::cout<<"Image Colour value at pixel (" << i<<","<<j<<") "<< pixel.r<<","<<pixel.g<< ","<<pixel.b<< std::endl;
        }
    }
    std::cout<<"Testing some images functions." << std::endl;*/
}

void CollisionHandler::loadTensor(){
    //Grabing images informations.
    int collisionWidth, collisionHeight;
    collisionWidth = static_cast<Ogre::uint32> (collisionTexture->getWidth());
    collisionHeight = static_cast<Ogre::uint32> (collisionTexture->getHeight());

    //Allocating terrainMatrix
    for (int i = 0; i < collisionWidth; i++) {
        std::vector<Textures> row;
        for (int j = 0; j < collisionHeight; j++) {
            row.push_back(TEX_NONE);
        }
        terrainMatrix.push_back(row);
    }

    //Pixel color to both images.
    Ogre::ColourValue collisionPixel;
    //Reading (per rows) images for data.
    for(int w = 0; w < collisionHeight; w++){
        for(int h = 0; h < collisionWidth; h++){
            collisionPixel = collisionTexture->getColourAt(w,h,0);    //receives collor from image
            Textures textureType = TEX_NONE;
            //const Ogre::ColourValue color = static_cast<const Ogre::ColourValue>(collisionPixel);
            if (collisionPixel == COL_WATER)
                textureType = TEX_WATER;
            else if(collisionPixel == COL_ROAD)
                textureType = TEX_ROAD;
            else if(collisionPixel == COL_ROCK)
                textureType = TEX_ROCK;
            else if(collisionPixel == COL_BULLET)
                textureType = TEX_BULLET;
            else if(collisionPixel == COL_GUAGMIRE)
                textureType = TEX_GUAGMIRE;
            terrainMatrix[w][h] = textureType;
            std::cout << terrainMatrix[w][h] << " ";
            }
            std::cout << std::endl;
        }
    }
}
