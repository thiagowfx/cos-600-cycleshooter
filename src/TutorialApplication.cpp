/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
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

#include "TutorialApplication.h"

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
    physicsIntegration = new OgreBulletIntrospection();
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
    delete mInfoLabel;
    delete physicsIntegration;
}

//---------------------------------------------------------------------------
void TutorialApplication::createScene(void) {

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

    Ogre::Light* mainLight = mSceneMgr->createLight("MainLight");
    mainLight->setType(Ogre::Light::LT_POINT);
    mainLight->setPosition(Ogre::Vector3(1000.0, 1000.0, 1000.0));


    Ogre::String sphereName = "Sphere";
    Ogre::Entity* sphereEntity = mSceneMgr->createEntity(sphereName, "sphere.mesh");
    sphereEntity->setMaterialName("Examples/SphereMappedRustySteel");

    Ogre::SceneNode* sphereNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(sphereName);
    sphereNode->attachObject(sphereEntity);
    sphereNode->setPosition(0.0, 1000.0, 0.0);

    // Create a Light and set its position

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


    //Spot Light.
    Ogre::Light* spotLight = mSceneMgr->createLight("spotlLight");
    spotLight->setType(Ogre::Light::LT_DIRECTIONAL);
    spotLight->setDiffuseColour(0,0,1.0);
    spotLight->setSpecularColour(0,0,1.0);
    spotLight->setDirection(-1,-1,0);
    spotLight->setPosition(300,300,0);
    //Defines the transversal section's form.
    spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));


    //Creating plane as explained in tutorial 2.
//    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0.0);
//    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
//        plane, 3000, 3000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

//    Ogre::String groundName = "Ground";
//    Ogre::Entity* groundEntity = mSceneMgr->createEntity(groundName, "ground");
//    groundEntity->setMaterialName("Examples/Rockwall");

//    Ogre::SceneNode* groundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(groundName);
//    groundNode->attachObject(groundEntity);


//    mCamera->setPosition(Ogre::Vector3(1000.0, 1000.0, 0.0));
//    mCamera->lookAt(sphereNode->getPosition());

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

    //headNode->translate(mTerrainGroup->getOrigin());
    //headNode->translate(.0,500.0,.0);
    //headNode->scale(50.0,50.0,50.0);

    mTerrainGroup->freeTemporaryResources();


    // physics
//    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0.0, 1.0, 0.0), 0.0);
//    btScalar groundMass = 0.0; // non-movable
//    btVector3 groundLocalInertia(0.0, 0.0, 0.0);
//    groundShape->calculateLocalInertia(groundMass, groundLocalInertia);

//    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0, 0.0, 0.0, 1.0)));
//    btRigidBody* groundBody = new btRigidBody(groundMass, groundMotionState, groundShape, groundLocalInertia);
//    groundBody->setUserPointer(groundNode);
//    physicsIntegration->getDynamicsWorld()->addRigidBody(groundBody);

    //btCollisionShape* terrainShape = new btHeightFieldTerrainShape();


    btCollisionShape* sphereShape = new btSphereShape(102);
    btScalar sphereMass = 0.1;
    btVector3 sphereLocalInertia(0.0, 0.0, 0.0);
    sphereShape->calculateLocalInertia(sphereMass, sphereLocalInertia);

    btTransform sphereStartTransform;
    sphereStartTransform.setRotation(btQuaternion(1.0, 1.0, 1.0, 0.0));
    sphereStartTransform.setOrigin(btVector3(0.0, 1000.0, 0.0));
    btDefaultMotionState *sphereMotionState = new btDefaultMotionState(sphereStartTransform);

    btRigidBody* sphereBody = new btRigidBody(sphereMass, sphereMotionState, sphereShape, sphereLocalInertia);
    sphereBody->setRestitution(1.0);
    sphereBody->setUserPointer(sphereNode);
    physicsIntegration->getDynamicsWorld()->addRigidBody(sphereBody);
}

void TutorialApplication::createFrameListener(void){
    BaseApplication::createFrameListener();
    mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "topLabel", "", 250.0);
    mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent &evt){
    if (physicsIntegration != NULL){
        physicsIntegration->getDynamicsWorld()->stepSimulation(1.0f / 60.0f);

        for (unsigned i = 0; i < physicsIntegration->getDynamicsWorld()->getCollisionObjectArray().size(); ++i) {
            btCollisionObject* obj = physicsIntegration->getDynamicsWorld()->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);

            if (body && body->getMotionState()){
                btTransform trans;
                body->getMotionState()->getWorldTransform(trans);

                void *userPointer = body->getUserPointer();
                if (userPointer) {
                    btQuaternion orientation = trans.getRotation();
                    Ogre::SceneNode *sceneNode = static_cast<Ogre::SceneNode*>(userPointer);
                    sceneNode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
                    sceneNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
                }
            }
        }
    }

    mInfoLabel->show();
    mInfoLabel->setCaption("Num objects: " + Ogre::StringConverter::toString(mWindow->getAverageFPS()));

    bool ret = BaseApplication::frameRenderingQueued(evt);
    if (mTerrainGroup->isDerivedDataUpdateInProgress())
    {
        mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
        mInfoLabel->show();
        if (mTerrainsImported)
        {
            mInfoLabel->setCaption("Building terrain, please wait...");
        }
        else
        {
            mInfoLabel->setCaption("Updating textures, patience...");
        }
    }
    else
    {
        mTrayMgr->removeWidgetFromTray(mInfoLabel);
        mInfoLabel->hide();
        if (mTerrainsImported)
        {
            mTerrainGroup->saveAllTerrains(true);
            mTerrainsImported = false;
        }
    }

    return ret;
}

void TutorialApplication::defineTerrain(long x, long y){
    Ogre::String filename = mTerrainGroup->generateFilename(x, y);
    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
    {
        mTerrainGroup->defineTerrain(x, y);
    }
    else
    {
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
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
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
    if(flipX) img.flipAroundX();
    if(flipY) img.flipAroundY();
}


bool TutorialApplication::configure() {
    if(mRoot->restoreConfig() || mRoot->showConfigDialog()) {
        mWindow = mRoot->initialise(true, cWindowTitle);
        return true;
    }
    else {
        return false;
    }
}

//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
