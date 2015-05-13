#ifndef _RESOURCES_HPP_
#define _RESOURCES_HPP_

#include "OgreConfigFile.h"
#include "OgreResourceGroupManager.h"

namespace Cycleshooter {
class Resources {
    Ogre::String resourcesConfig;
    Ogre::String pluginsConfig;

public:
    Resources(Ogre::String pluginsConfig = "plugins.cfg", Ogre::String resourcesConfig = "resources.cfg");

    void setupResources();
    Ogre::String getPluginConfig() const;
    Ogre::String getResourcesConfig() const;
};

}

#endif
