#include "CollisionHandler.hpp"

namespace Cycleshooter {


CollisionHandler::CollisionHandler(Ogre::String collisionTexturePath):
    collisionTexture(0),
    collisionTexturePath(collisionTexturePath),
    collisionMatrixWidth(0),
    collisionMatrixHeight(0)
{
//    collisionMap = {{WATER_COLOR,WATER_PIXEL},{BULLET_COLOR,BULLET_PIXEL},
//                    {ROCK_COLOR,ROCK_PIXEL},{GUAGMIRE_COLOR,GUAGMIRE_PIXEL},
//                    {ROAD_COLOR,ROAD_PIXEL}};
}

CollisionHandler::~CollisionHandler() {
    if(collisionTexture)
        delete collisionTexture;
}

void CollisionHandler::loadImages(){
    collisionTexture = new Ogre::Image();
    collisionTexture->load(collisionTexturePath, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    //Loading Matrix Dimensions.
    collisionMatrixWidth = static_cast<int>(collisionTexture->getWidth());
    collisionMatrixHeight = static_cast<int>(collisionTexture->getHeight());
}

void CollisionHandler::loadTensor(){
    loadImages();
    // Allocating terrainMatrix
    collisionMatrix = std::vector< std::vector<Colors> >(collisionMatrixWidth, std::vector<Colors>(collisionMatrixHeight, NONE_PIXEL));
    //Pixel color to both images.
    Ogre::ColourValue collisionPixel;
    //Reading (per rows) images for data.
    for(int row = 0; row < collisionMatrixWidth; row++){
        for(int col = 0; col < collisionMatrixHeight; col++){
            collisionPixel = collisionTexture->getColourAt(row, col, 0);    //Receives collor from image
            //std::cout <<"Image color at " <<col << "," << row <<" "<< Ogre::StringConverter::toString(collisionPixel)<<std::endl;
            //const Ogre::ColourValue color = static_cast<const Ogre::ColourValue>(collisionPixel);
            if (collisionPixel == WATER_COLOR)
                collisionMatrix[row][col] = WATER_PIXEL;
            else if(collisionPixel.r == collisionPixel.g && collisionPixel.b == collisionPixel.g)
                collisionMatrix[row][col] = ROAD_PIXEL;
            else if(collisionPixel == ROCK_COLOR)
                collisionMatrix[row][col] = ROCK_PIXEL;
            else if(collisionPixel == BULLET_COLOR)
                collisionMatrix[row][col] = BULLET_PIXEL;
            else if(collisionPixel == GUAGMIRE_COLOR)
                collisionMatrix[row][col] = GUAGMIRE_PIXEL;
            else if(collisionPixel == START_COLOR)
                collisionMatrix[row][col] = START_PIXEL;
            }
    }
    printMatrix();
}

void CollisionHandler::printMatrix()
{
    int matrixColNumber, matrixRowNumber;
    //Grabing matrix dimensions.
    matrixColNumber = collisionMatrix[0].size();
    matrixRowNumber = collisionMatrix.size();
//    for(int pixelWidth = 0;pixelWidth < matrixColNumber;pixelWidth++){
//        for(int pixelHeight = 0; pixelHeight < matrixRowNumber;pixelHeight++){
//            std::cout << "Collision Matrix at position (" << pixelWidth << "," << pixelHeight << ")";
//            std::cout << " = " << collisionMatrix[pixelHeight][pixelWidth] << std::endl;
//        }
//    }
    std::cout << "Collision Matrix Dimensions " << matrixRowNumber << "," << matrixColNumber << std::endl;
}

bool CollisionHandler::testMatrixDimension()
{
    if(collisionMatrix[0].size() == collisionMatrixWidth && collisionMatrix.size() == collisionMatrixHeight)
        return true;
    std::cout << "Error while allocating collision matrix" << std::endl;
    return false;
}

CollisionHandler::Colors CollisionHandler::getPixelEnumeration(int pixelWidth, int pixelHeight){
    return collisionMatrix[pixelWidth][pixelHeight];
}

std::pair<int,int> CollisionHandler::getStartPixel(){
    int matrixColNumber, matrixRowNumber;
    //Grabing matrix dimensions.
    matrixColNumber = collisionMatrix[0].size();
    matrixRowNumber = collisionMatrix.size();
    std::pair<int,int> pixelLocation;
    //Serching the matrix.
    for(int pixelWidth = 0;pixelWidth<matrixColNumber;pixelWidth++){
        for(int pixelHeight = 0; pixelHeight < matrixRowNumber;pixelHeight++){
            if(collisionMatrix[pixelHeight][pixelWidth] == START_PIXEL){
                pixelLocation = std::make_pair (pixelWidth,pixelHeight);
                return pixelLocation;
            }
        }
    }
    //Error return state.
    pixelLocation = std::make_pair (-1,-1);
    return pixelLocation;
}

std::vector<std::vector<CollisionHandler::Colors> > CollisionHandler::getCollisionMatrix(){
    return collisionMatrix;
}

int CollisionHandler::getCollisionMatrixWidth() const{
    return collisionMatrixWidth;
}

int CollisionHandler::getCollisionMatrixHeight() const{
    return collisionMatrixHeight;
}

}
