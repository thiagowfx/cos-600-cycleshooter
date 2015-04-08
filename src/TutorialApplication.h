/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include "OgreBulletIntrospection.h"

#include <iostream>

//---------------------------------------------------------------------------

class TutorialApplication : public BaseApplication
{
private:
    void defineTerrain(long x, long y);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults(Ogre::Light* light);
    void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);

    Ogre::TerrainGlobalOptions* mTerrainGlobals;
    Ogre::TerrainGroup* mTerrainGroup;
    bool mTerrainsImported;
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);

protected:
    // overriden from BaseApplication
    virtual bool configure(void);
    virtual void createFrameListener(void);
    virtual void createScene(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);

    // configurations
    std::string cApplicationName = "OgreBulletApp";
    std::string cWindowTitle = cApplicationName + " Render Window";
    OgreBites::Label* mInfoLabel;
    OgreBulletIntrospection* physicsIntegration;
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
