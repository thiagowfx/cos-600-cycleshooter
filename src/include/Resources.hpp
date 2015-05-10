#ifndef _RESOURCES_HPP_
#define _RESOURCES_HPP_

#include "OgreConfigFile.h"
#include "OgreResourceGroupManager.h"

namespace Cycleshooter {
class Resources {
    Ogre::String resourcesConfig;
    Ogre::String pluginsConfig;
    Ogre::String resourcesPath; // For OS X compatibility

public:
    Resources(Ogre::String pluginsConfig, Ogre::String resourcesConfig);

    void setupResources();
    Ogre::String getPluginConfig() const;
    Ogre::String getResourcesConfig() const;
};

}

#endif
