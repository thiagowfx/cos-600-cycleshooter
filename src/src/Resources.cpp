#include "Resources.hpp"

namespace Cycleshooter {

Resources::Resources(Ogre::String pluginsConfig, Ogre::String resourcesConfig) :
    pluginsConfig(pluginsConfig),
    resourcesConfig(resourcesConfig),
    resourcesPath(Ogre::StringUtil::BLANK)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    resourcesPath = Ogre::macBundlePath() + "/Contents/Resources/";
#endif

#ifndef OGRE_STATIC_LIB
    resourcesConfig = resourcesPath + resourcesConfig;
    pluginsConfig = resourcesPath + pluginsConfig;
#endif
}

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

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            // adjust working directory
            if (!Ogre::StringUtil::startsWith(archName, "/", false))
                archName = Ogre::String(Ogre::macBundlePath() + "/" + archName);
#endif

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
