/*
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

#include "TutorialApplication.h"


TutorialApplication::TutorialApplication(void)
{
}

TutorialApplication::~TutorialApplication(void)
{
}


void TutorialApplication::createScene(void)
{
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

    // Create an Entity
    Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");
    //Ogre::Entity* ogreHead2 = mSceneMgr->createEntity("Head2", "ogrehead.mesh");

    //Creating a GroundPlane
//    Ogre::Plane plane(Ogre::Vector3::UNIT_Y,0);
//    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
//        plane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
//    Ogre::Entity* ground = mSceneMgr->createEntity("GroundEntity","ground");
//    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ground);
//    ground->setMaterialName("Examples/Rockwall");
//    ground->setCastShadows(true);
 
    // Create a SceneNode and attach the Entity to it
    Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HeadNode");
    headNode->attachObject(ogreHead);
 
    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(20.0f, 80.0f, 50.0f);

    //mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

    //Lights Section

    //The Light point of the scene.
    Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(0,120,50);
    pointLight->setDiffuseColour(0.2038,0.503,0.576);
    pointLight->setSpecularColour(0.2038,0.503,0.576);

    //Directional Light.
    Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();
    Ogre::Light* directionalLight = mSceneMgr->createLight("directionalLight");
    directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
    directionalLight->setDiffuseColour(0.4,0.039,0.500);
    directionalLight->setSpecularColour(0.003,0.039,0.500);
    //Direction defined to adjust to the terrain.
    directionalLight->setDirection(lightdir);

    //Spot Light
    Ogre::Light* spotLight = mSceneMgr->createLight("spotlLight");
    spotLight->setType(Ogre::Light::LT_DIRECTIONAL);
    spotLight->setDiffuseColour(0,0,1.0);
    spotLight->setSpecularColour(0,0,1.0);
    spotLight->setDirection(-1,-1,0);
    spotLight->setPosition(300,300,0);
    //Defines the transversal section's form.
    spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));

    //mSceneMgr->setSkyDome(true,"Examples/CloudySky",5,8);
    //mSceneMgr->setSkyBox(true,"Examples/SpaceSkyBox");

    //Terrain camera modifications.
    //The attributes should be modified due to the size of the terrain.
    mCamera->setPosition(Ogre::Vector3(1683, 50, 2116));
    mCamera->lookAt(Ogre::Vector3(1963, 50, 1660));
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(50000);

    if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE)){
        mCamera->setFarClipDistance(0);   // enable infinite far clip distance if possible
    }
    Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);

    //Starting terrain itself.
    mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr,Ogre::Terrain::ALIGN_X_Z,513,12000.0f);

    mTerrainGroup->setFilenameConvention(Ogre::String("BasicTutorial3Terrain"), Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

    configureTerrainDefaults(directionalLight);

    //Visiting terraing to definition through coordinates.
    for (long x = 0; x <= 0; ++x)
        for (long y = 0; y <= 0; ++y)
            defineTerrain(x, y);

    // sync load since we want everything in place when we start
    mTerrainGroup->loadAllTerrains(true);

    if (mTerrainsImported)
    {
        Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Ogre::Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }

    headNode->translate(mTerrainGroup->getOrigin());
    headNode->translate(.0,500.0,.0);
    //headNode->scale(50.0,50.0,50.0);

    mTerrainGroup->freeTemporaryResources();
}

void TutorialApplication::destroyScene(){
    OGRE_DELETE mTerrainGroup;
    OGRE_DELETE mTerrainGlobals;
}

void TutorialApplication::createFrameListener(void){
    BaseApplication::createFrameListener();
    mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TInfo", "", 350);
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent &evt){
    bool ret = BaseApplication::frameRenderingQueued(evt);

    if (mTerrainGroup->isDerivedDataUpdateInProgress()) {
        mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
        mInfoLabel->show();
        if (mTerrainsImported) {
            mInfoLabel->setCaption("Building terrain, please wait...");
        }
        else {
            mInfoLabel->setCaption("Updating textures, patience...");
        }
    }
    else {
        mTrayMgr->removeWidgetFromTray(mInfoLabel);
        mInfoLabel->hide();
        if (mTerrainsImported) {
            mTerrainGroup->saveAllTerrains(true);
            mTerrainsImported = false;
        }
    }

    return ret;
}

void TutorialApplication::createViewports(){
    //Creating Viewport to mCamera.
    Ogre::Viewport* viewport = mWindow->addViewport(mCamera);
    viewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
    mCamera->setAspectRatio(Ogre::Real(viewport->getActualWidth())/Ogre::Real(viewport->getActualHeight()));

}

void TutorialApplication::createCamera(){
    mCamera = mSceneMgr->createCamera("playercam");

    //Setting camera atributes.
    mCamera->setPosition(0.0,10.0,500.0);
    mCamera->lookAt(0,0,0);
    mCamera->setNearClipDistance(5);

    //Creating playercam cameraman.
    mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}

void TutorialApplication::defineTerrain(long x, long y){
    Ogre::String filename = mTerrainGroup->generateFilename(x, y);

    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename)) {
        mTerrainGroup->defineTerrain(x, y);
    }
    else {
        Ogre::Image img;
        getTerrainImage(x % 2 != 0, y % 2 != 0, img);
        mTerrainGroup->defineTerrain(x, y, &img);
        mTerrainsImported = true;
    }
}

void TutorialApplication::initBlendMaps(Ogre::Terrain* terrain){
    Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);

    Ogre::Real minHeight0 = 70;
    Ogre::Real fadeDist0 = 40;
    Ogre::Real minHeight1 = 70;
    Ogre::Real fadeDist1 = 15;

    float* pBlend0 = blendMap0->getBlendPointer();
    float* pBlend1 = blendMap1->getBlendPointer();

    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y) {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x) {
            Ogre::Real tx, ty;

            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend0++ = val;

            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;
        }
    }

    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update();
}

void TutorialApplication::configureTerrainDefaults(Ogre::Light* light){
    // Configure global
    mTerrainGlobals->setMaxPixelError(8);
    // testing composite map
    mTerrainGlobals->setCompositeMapDistance(3000);

    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
    mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
    mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
    mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());

    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = 513;
    defaultimp.worldSize = 12000.0f;
    defaultimp.inputScale = 600;
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;

    // textures
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 100;
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 30;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
}

void TutorialApplication::getTerrainImage(bool flipX, bool flipY, Ogre::Image &img){
    img.load("terrain.png",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    if(flipX)
        img.flipAroundX();

    if(flipY)
        img.flipAroundY();
}

#ifdef __cplusplus
extern "C" {
#endif

    int main(int argc, char *argv[])
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
