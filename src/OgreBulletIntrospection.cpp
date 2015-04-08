#include "OgreBulletIntrospection.h"

OgreBulletIntrospection::OgreBulletIntrospection() {
    initObjects();
}

OgreBulletIntrospection::~OgreBulletIntrospection() {
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
}

void OgreBulletIntrospection::initObjects() {
    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver();

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0.0, -10.0, 0.0));
}

btDiscreteDynamicsWorld *OgreBulletIntrospection::getDynamicsWorld() {
    return dynamicsWorld;
}
