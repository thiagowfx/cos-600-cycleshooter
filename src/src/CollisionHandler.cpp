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
    collisionTexture = new Ogre::Image();
    collisionTexture->load(collisionTexturePath, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    Ogre::uint32 imgW, imgH;
    imgW = collisionTexture->getWidth();
    imgH = collisionTexture->getHeight();
}

void CollisionHandler::loadTensor(){
    //Grabing images informations.

    int collisionWidth, collisionHeight;
    collisionWidth = static_cast<int>(collisionTexture->getWidth());
    collisionHeight = static_cast<int>(collisionTexture->getHeight());

    // allocating terrainMatrix
    terrainMatrix = std::vector< std::vector<Textures> >(collisionHeight, std::vector<Textures>(collisionWidth, TEX_NONE));

    //Pixel color to both images.
    Ogre::ColourValue collisionPixel;
    //Reading (per rows) images for data.
    for(unsigned row = 0; row < collisionHeight; row++){
        for(unsigned col = 0; col < collisionWidth; col++){
            collisionPixel = collisionTexture->getColourAt(col, row, 0);    //receives collor from image
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
            terrainMatrix[row][col] = textureType;
            // std::cout << terrainMatrix[row][col] << " ";
            }
            std::cout << std::endl;
        }
    }
}
