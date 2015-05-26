#include "TerrainManager.hpp"

namespace Cycleshooter {

TerrainManager::TerrainManager(Ogre::SceneManager* sceneManager, std::string collisionTexturePath):
    sceneManager(sceneManager),
    terrainSize(0),
    terrainWorldSize(0.0)
{
    //Initializing collision handler.
    collisionHandler = new collisionHandler(collisionTexturePath);
    collisionHandler->loadImages();
    collisionHandler->loadTensor();
}

TerrainManager::~TerrainManager() {
    OGRE_DELETE terrainGroup;
    OGRE_DELETE globalOptions;
}

void TerrainManager::createTerrainLight(){
    sceneManager->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
    //Defines light direction.
    Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();
    //Creates the light pointing to terrain.
    Ogre::Light* light = sceneManager->createLight("TerrainLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
}

void TerrainManager::createTerrain(){
    //Initializing a planar terrain structure.
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -100);
    Ogre::MeshManager::getSingleton().createPlane(
      "groundTerrain",
      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
      plane,
      10000, 10000, 20, 20,
      true,
      1, 1,1,
      Ogre::Vector3::UNIT_Z);

    //Transforming planar mesh to treatable entity.
    Ogre::Entity* groundEntity = sceneManager->createEntity("groundTerrain");

    //Adding entity to the scene.
    sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
    groundEntity->setCastShadows(false);

    //Defines which texture will be used. 
    groundEntity->setMaterialName("Examples/Ground");
}

}
