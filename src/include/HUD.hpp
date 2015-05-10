#ifndef _HUD_HPP_
#define _HUD_HPP_

#include <SFML/System/NonCopyable.hpp>

#include "InputContext.h"
#include "OgreCamera.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#  include <OGRE/SdkTrays.h>
#else
#  include <SdkTrays.h>
#endif

namespace Cycleshooter {
class HUD : sf::NonCopyable {
    OgreBites::SdkTrayManager* mTrayManager;
    OgreBites::ParamsPanel* mDebugPanel;

public:
    HUD(const Ogre::String& name, Ogre::RenderWindow* window, OgreBites::InputContext inputContext, OgreBites::SdkTrayListener* listener = 0);
    virtual ~HUD();

    void toggleDebugPanel();
    void toggleFPSPanel();
    void update(const Ogre::FrameEvent& evt);
    void updateDebugPanel_CameraElements(Ogre::Camera* camera);

    bool isDebugPanelVisible() const;
    std::string getDebugPanel_PolygonFilteringElement() const;
    void setDebugPanel_PolygonFilteringElement(const std::string& newVal);
    std::string getDebugPanel_PolygonRenderingElement() const;
    void setDebugPanel_PolygonRenderingElement(const std::string& newVal);
    OgreBites::SdkTrayManager *getTrayManager() const;
};

}

#endif
