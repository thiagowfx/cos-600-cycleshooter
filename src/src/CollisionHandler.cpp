#include "CollisionHandler.hpp"

namespace Cycleshooter {


CollisionHandler::CollisionHandler(Ogre::String collisionTexturePath):
    collisionTexture(0),
    collisionTexturePath(collisionTexturePath),
    collisionMatrixWidth(0),
    collisionMatrixHeight(0),
    bulletCount(0)
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
    bulletMatrix = std::vector<std::vector<std::pair<bool, BulletElement> > > (collisionMatrixWidth,std::vector<std::pair<bool, BulletElement> >(collisionMatrixHeight,std::make_pair(false,BulletElement())));
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
            else if(collisionPixel == START_COLOR)
                collisionMatrix[row][col] = START_PIXEL;
            else if(collisionPixel.r == collisionPixel.g && collisionPixel.b == collisionPixel.g)
                collisionMatrix[row][col] = ROAD_PIXEL;
            else if(collisionPixel == ROCK_COLOR)
                collisionMatrix[row][col] = ROCK_PIXEL;
            else if(collisionPixel == BULLET_COLOR)
                collisionMatrix[row][col] = BULLET_PIXEL;
            else if(collisionPixel == GUAGMIRE_COLOR)
                collisionMatrix[row][col] = GUAGMIRE_PIXEL;
            }
    }
    //printMatrix();
    //printBullets();
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
//    std::cout << "Collision Matrix Dimensions " << matrixRowNumber << "," << matrixColNumber << std::endl;
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

std::pair<bool,Ogre::String> CollisionHandler::isBulletAt(int pixelWidth, int pixelHeight,Ogre::Vector3 coord,Ogre::Real radius){
    //Grabbing correspondent datastructure position.
    BulletElement e = bulletMatrix[pixelWidth][pixelHeight].second;
    bool test = bulletMatrix[pixelWidth][pixelHeight].first;
    if(!test)
        return std::make_pair(false,e.getScenenodeName());
    //Defining collision equation(bullets are spheres).
    Ogre::Vector3 center = e.getCoordinate();
    //Defining sphere partial definitions.
    Ogre::Real tx = coord.x-center.x;
    Ogre::Real ty = coord.y-center.y;
    Ogre::Real tz = coord.z-center.z;
    tx = tx * tx;
    ty = ty * ty;
    tz = tz * tz;
    Ogre::Real sum = tx + ty + tz;
    //Comparing sphere discriminant with radius
    bool belongsToSphere = sum <= radius * radius;
    //Logically removing bullet from the scene.
    if(belongsToSphere){
        bulletMatrix[pixelWidth][pixelHeight].first = false;
        return std::make_pair(true,e.getScenenodeName());
    }
    else
        return std::make_pair(false,e.getScenenodeName());
    //return bulletMatrix[pixelWidth][pixelHeight].first;
}

//TODO: change this to be called by isBullet().
//This responsability lay upon CollisionHanlder, not TerrainManager.
void CollisionHandler::removeBullet(int pixelWidth, int pixelHeight){
    Ogre::LogManager::getSingletonPtr()->logMessage("--> CollisioHandler: Removing Bullet State! <--");
    //int sizeW = bulletMatrix.size();
    //int sizeH = bulletMatrix[0].size();
    //std::cout<<"Total w = "<< sizeW << std::endl;
    //bulletMatrix[pixelWidth][pixelHeight].first = false;
}

void CollisionHandler::setBulletAt(int width, int height,bool exist, Ogre::Vector3 coord){
    //if(bulletMatrix[width][height].first) std::cout << "There is a bullet here!" << std::endl;
    //Setting the new bullet.
    bulletMatrix[width][height].first = true;
    bulletMatrix[width][height].second.setNewBullet(coord,bulletCount);
    BulletElement test = bulletMatrix[width][height].second;
    std::cout << "Testing name " << test.getScenenodeName()<<std::endl;
    //Increasing bullet identifiers.
    bulletCount++;
}

std::pair<std::vector<Ogre::String> , std::vector<Ogre::Vector3> > CollisionHandler::getSceneNodeNames(){
    std::vector<Ogre::String> names;
    std::vector<Ogre::Vector3> coords;
    int matrixColNumber, matrixRowNumber;
    //Grabing matrix dimensions.
    matrixColNumber = bulletMatrix[0].size();
    matrixRowNumber = bulletMatrix.size();
    //printBullets();
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

}
