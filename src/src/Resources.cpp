#include "Resources.hpp"

namespace Cycleshooter {

Resources::Resources(Ogre::String pluginsConfig, Ogre::String resourcesConfig) :
    pluginsConfig(pluginsConfig),
    resourcesConfig(resourcesConfig)
{}

void Resources::setupResources() {

    // load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(resourcesConfig);

    // go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements()) {
        secName = seci.peekNextKey();

        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;

        for (i = settings->begin(); i != settings->end(); ++i) {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
}

Ogre::String Resources::getPluginConfig() const {
    return pluginsConfig;
}

Ogre::String Resources::getResourcesConfig() const {
    return resourcesConfig;
}

}
