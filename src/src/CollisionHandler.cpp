#include "CollisionHandler.hpp"

namespace Cycleshooter {


CollisionHandler::CollisionHandler(Ogre::String collisionTexturePath):
    collisionTexture(0),
    collisionTexturePath(collisionTexturePath),
    collisionMatrixWidth(0),
    collisionMatrixHeight(0),
    bulletCount(0)
{

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
    bulletMatrix = std::vector<std::vector<std::pair<bool, BulletElement> > > (collisionMatrixWidth,std::vector<std::pair<bool, BulletElement> >(collisionMatrixHeight,std::make_pair(false,BulletElement())));

    //Pixel color to both images.
    Ogre::ColourValue collisionPixel;

    //Reading (per rows) images for data.
    for(int row = 0; row < collisionMatrixWidth; row++){
        for(int col = 0; col < collisionMatrixHeight; col++){
            collisionPixel = collisionTexture->getColourAt(row, col, 0);

            // Circuit Materials
            if (collisionPixel == ROAD_COLOR)
                collisionMatrix[row][col] = ROAD_PIXEL;
            else if(collisionPixel == ROCK_COLOR)
                collisionMatrix[row][col] = ROCK_PIXEL;
            else if(collisionPixel == SAND_COLOR)
                collisionMatrix[row][col] = SAND_PIXEL;
            else if(collisionPixel == ICE_COLOR)
                collisionMatrix[row][col] = ICE_PIXEL;

            // Outside Circuit Materials
            else if(collisionPixel == START_COLOR)
                collisionMatrix[row][col] = START_PIXEL;
            else if (collisionPixel == PATH_COLOR)
                collisionMatrix[row][col] = PATH_PIXEL;
            else if(collisionPixel == GRASS_COLOR)
                collisionMatrix[row][col] = GRASS_PIXEL;
        }
    }
}

void CollisionHandler::printMatrix() {
    int matrixColNumber, matrixRowNumber;

    //Grabing matrix dimensions.
    matrixColNumber = collisionMatrix[0].size();
    matrixRowNumber = collisionMatrix.size();
}

void CollisionHandler::printBullets(){
    int matrixColNumber, matrixRowNumber;

    //Grabing matrix dimensions.
    matrixColNumber = bulletMatrix[0].size();
    matrixRowNumber = bulletMatrix.size();
    for(int pixelWidth = 0;pixelWidth < matrixRowNumber;pixelWidth++){
        for(int pixelHeight = 0; pixelHeight < matrixColNumber;pixelHeight++){
            if(bulletMatrix[pixelWidth][pixelHeight].first){
                //std::cout << "Bullet Matrix at (" << pixelWidth << "," << pixelHeight << ")"<< " exists!" << std::endl;
                std::cout << bulletMatrix[pixelWidth][pixelHeight].second.getScenenodeName() << std::endl;
            }
            std::cout << bulletMatrix[pixelWidth][pixelHeight].first << std::endl;
        }
    }
}

bool CollisionHandler::testMatrixDimension() {

    if(collisionMatrix[0].size() == collisionMatrixWidth && collisionMatrix.size() == collisionMatrixHeight)
        return true;
    std::cout << "Error while allocating collision matrix" << std::endl;

    return false;
}

Colors CollisionHandler::getPixelEnumeration(int pixelWidth, int pixelHeight){
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
            if(collisionMatrix[pixelWidth][pixelHeight] == START_PIXEL){
                pixelLocation = std::make_pair (pixelWidth,pixelHeight);
                return pixelLocation;
            }
        }
    }

    //Error return state.
    pixelLocation = std::make_pair (-1,-1);
    return pixelLocation;
}

std::vector<std::vector<Colors> > CollisionHandler::getCollisionMatrix(){
    return collisionMatrix;
}

int CollisionHandler::getCollisionMatrixWidth() const{
    return collisionMatrixWidth;
}

int CollisionHandler::getCollisionMatrixHeight() const{
    return collisionMatrixHeight;
}

bool CollisionHandler::existBulletAt(int pixelWidth, int pixelHeight){
    return bulletMatrix[pixelWidth][pixelHeight].first;
}

std::pair<Ogre::Vector3,Ogre::String> CollisionHandler::getBulletPropertiesAt(int pixelWidth, int pixelHeight){
    Ogre::LogManager::getSingletonPtr()->logMessage("--> CollisionHandler: getBulletNameAt <--");
    Ogre::LogManager::getSingletonPtr()->logMessage(bulletMatrix[pixelWidth][pixelHeight].second.getScenenodeName());

    return std::make_pair(bulletMatrix[pixelWidth][pixelHeight].second.getCoordinate(),bulletMatrix[pixelWidth][pixelHeight].second.getScenenodeName());
}

std::vector<std::pair<int, int> > CollisionHandler::getPathControllPoints(){
    int matrixColNumber, matrixRowNumber;

    //Grabing matrix dimensions.
    matrixColNumber = collisionMatrix[0].size();
    matrixRowNumber = collisionMatrix.size();
    std::vector<std::pair<int,int> > pointsLocation;

    //Serching the matrix.
    for(int pixelWidth = 0;pixelWidth<matrixColNumber;pixelWidth++){
        for(int pixelHeight = 0; pixelHeight < matrixRowNumber;pixelHeight++){
            if(collisionMatrix[pixelWidth][pixelHeight] == PATH_PIXEL){
                pointsLocation.push_back(std::pair<int,int> (pixelWidth,pixelHeight));
            }
        }
    }
    return pointsLocation;

}

void CollisionHandler::insertBulletAt(int width, int height,bool exist, Ogre::Vector3 coord){
    //Setting the new bullet.
    bulletMatrix[width][height].first = true;
    bulletMatrix[width][height].second.setNewBullet(coord,bulletCount);
    BulletElement test = bulletMatrix[width][height].second;
    std::cout << "Testing name " << test.getScenenodeName()<<std::endl;
    std::cout << "Position " << width << "," << height<<std::endl;
    //Increasing bullet identifiers.
    bulletCount++;
    std::cout << "Number of Bullets = " <<bulletCount << std::endl;
}

void CollisionHandler::setBulletState(int width, int height, bool state){
    bulletMatrix[width][height].first = state;
}

std::pair<std::vector<Ogre::String> , std::vector<Ogre::Vector3> > CollisionHandler::getBulletsForRender(){
    std::vector<Ogre::String> names;
    std::vector<Ogre::Vector3> coords;
    int matrixColNumber, matrixRowNumber;

    //Grabing matrix dimensions.
    matrixColNumber = bulletMatrix[0].size();
    matrixRowNumber = bulletMatrix.size();
    Ogre::LogManager::getSingletonPtr()->logMessage("--> CollisionHandler: Searching Bullets <--");
    for(int pixelWidth = 0;pixelWidth < matrixRowNumber;pixelWidth++){
        for(int pixelHeight = 0; pixelHeight < matrixColNumber;pixelHeight++){
            if(bulletMatrix[pixelWidth][pixelHeight].first){
                Ogre::LogManager::getSingletonPtr()->logMessage("--> CollisionHandler: Found Bullet <--");
                BulletElement e = bulletMatrix[pixelWidth][pixelHeight].second;
                Ogre::String s = e.getScenenodeName();
                Ogre::Vector3 v = e.getCoordinate();
                names.push_back(s);
                coords.push_back(v);
            }
        }
    }
    return std::make_pair(names, coords);
}

void CollisionHandler::compensateBulletRender(std::vector<std::pair<int, int> > coords){
    int cWidth = coords[0].first;
    int cHeight = coords[0].second;
    int width, height;
    Ogre::LogManager::getSingletonPtr()->logMessage("--> CollisionHandler: Compensating Bullet Render <--");
    Ogre::LogManager::getSingletonPtr()->logMessage(bulletMatrix[cWidth][cHeight].second.getScenenodeName());
    for(int i = 1;i < coords.size();i++){
        width = coords[i].first;
        height = coords[i].second;
        bulletMatrix[width][height].first = true;
        bulletMatrix[width][height].second = bulletMatrix[cWidth][cHeight].second;
        Ogre::LogManager::getSingletonPtr()->logMessage(bulletMatrix[cWidth][cHeight].second.getScenenodeName());
    }
}

double CollisionHandler::setOgreScale(int x){
    return x / 255.0;
}
}
