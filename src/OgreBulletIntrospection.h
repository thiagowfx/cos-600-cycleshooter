#ifndef _BULLET_IMPORTS_H_
#define _BULLET_IMPORTS_H_

#include <btBulletDynamicsCommon.h>

#include <OgreSceneManager.h>

class OgreBulletIntrospection {
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
public:
    OgreBulletIntrospection();
    virtual ~OgreBulletIntrospection();

    void initObjects();
    btDiscreteDynamicsWorld* getDynamicsWorld();
};

#endif
