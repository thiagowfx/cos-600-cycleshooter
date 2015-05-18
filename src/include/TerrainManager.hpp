#ifndef _TERRAINMANAGER_HPP_
#define _TERRAINMANAGER_HPP_

#include<OgreSceneManager.h>
#include<OgreEntity.h>
#include<OgreMeshManager.h>
#include<OgreString.h>

namespace Cycleshooter {
class TerrainManager {
private:
    Ogre::SceneManager* sceneManager;
    Ogre::String terrainType;
public:
    //Class to define game terrain.
    TerrainManager(Ogre::SceneManager* sceneManager);
    virtual ~TerrainManager();

    virtual void createTerrain();
};

}

#endif
