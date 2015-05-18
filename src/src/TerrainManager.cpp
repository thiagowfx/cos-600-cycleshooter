#include "TerrainManager.hpp"

namespace Cycleshooter {

TerrainManager::TerrainManager(Ogre::SceneManager* sceneManager):
    sceneManager(sceneManager)
{
}

TerrainManager::~TerrainManager() {
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
