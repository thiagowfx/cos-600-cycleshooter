#ifndef _TERRAINMANAGER_HPP_
#define _TERRAINMANAGER_HPP_

#include <Ogre.h>

namespace Cycleshooter {

class TerrainManager {
    Ogre::SceneManager* const sceneManager;

    // customizable settings
    Ogre::String terrainType = "Plane";

public:
    TerrainManager(Ogre::SceneManager* sceneManager);
    virtual ~TerrainManager();

    void createTerrain();
};

}

#endif
