#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include <SFML/System.hpp>
#include <Ogre.h>
#include <OgreOverlaySystem.h>

#include "Constants.hpp"
#include "Resources.hpp"
#include "NodeManager.hpp"

namespace Cycleshooter {
class NodeManager;

enum Context {
    CONTEXT_RUNNER,
    CONTEXT_SHOOTER,
    CONTEXT_NONE
};

class Controller : sf::NonCopyable {

    Context context;
    NodeManager* nodeManager = NULL;

    Ogre::Root *oRoot = NULL;
    Ogre::SceneManager *oSceneManager = NULL;
    Ogre::OverlaySystem *oOverlaySystem = NULL;
    Cycleshooter::Resources *oResources = NULL;

public:
    Controller();
    virtual ~Controller();

    void go();
    void createSceneManager();
    void createOverlaySystem();

    void setupRunnerMode();
    void setupShooterMode();
    void setupNoneMode();
    void toggleMode();
    void toggleMode(const Context& newContext);

    // getters and setters
    Context getContext() const;
    void setContext(const Context &value);
    NodeManager *getNodeManager() const;
    void setNodeManager(NodeManager *value);
    Ogre::Root *getRoot() const;
    void setRoot(Ogre::Root *value);
    Ogre::SceneManager *getSceneManager() const;
    void setSceneManager(Ogre::SceneManager *value);
    Cycleshooter::Resources *getResources() const;
    void setResources(Cycleshooter::Resources *value);
    Ogre::OverlaySystem *getOverlaySystem() const;
    void setOverlaySystem(Ogre::OverlaySystem *value);
};
}

#endif
