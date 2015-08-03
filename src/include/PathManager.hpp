#ifndef _PATHMANAGER_HPP_
#define _PATHMANAGER_HPP_

#include <vector>

#include <Ogre.h>
#include <OgreSceneManager.h>
#include <OgreSimpleSpline.h>

#include "ConfigManager.hpp"
#include "Logging.hpp"
#include "AudioManager.hpp"
#include "Context.hpp"

namespace Cycleshooter {

class PathManager: public Ogre::SimpleSpline {

    Ogre::Real epsilon = 0.0001;

    // Monster
    Ogre::Vector3 monsterTangent;
    Ogre::Real monsterParametricPosition = 0.0;
    Ogre::Real monsterSplineStep = 0.001;
    Ogre::Real monsterSplineVelocity = ConfigManager::instance().getDouble("PathManager.monster_spline_velocity_percent");
    unsigned monsterIndex = ConfigManager::instance().getInt("Controller.monster_start_index");
    unsigned monsterNextIndex = monsterIndex + 1;
    Ogre::Vector3 monsterNextPosition;
    bool isPlayerClose = false;
    Ogre::Real monsterVelocityIfCloseToPlayerRunner = ConfigManager::instance().getDouble("PathManager.monster_velocity_if_close_to_player_runner");
    Ogre::Real monsterVelocityIfCloseToPlayerShooter = ConfigManager::instance().getDouble("PathManager.monster_velocity_if_close_to_player_shooter");
    Ogre::Real MONSTER_CLOSE_MINIMUM_DISTANCE = ConfigManager::instance().getDouble("PathManager.monster_close_distance");

    void go(const std::vector<Ogre::Vector3>& controlPoints);

public:
    PathManager(const char* file);
    PathManager(const std::vector<Ogre::Vector3>& controlPoints);

    void monsterPathUpdate(Ogre::Real timeSinceLastFrame, const Ogre::Vector3& playerPosition, const Ogre::Vector3& monsterPosition, Context context);
    void updateIndex();
    void setDebug(bool debug);
    Ogre::Vector3 getMonsterTangent() const;
    Ogre::Vector3 getTangent(unsigned index, double t) const;
    Ogre::Vector3 getAntiTangentFromPoint(const Ogre::Vector3& point) const;
    Ogre::Vector3 getMonsterNextPosition() const;
    void setMonsterNextPosition(const Ogre::Vector3 &value);

    /**
     * Binary searches the nearest point on the spline in relation to the specified point.
     * Time complexity: O(mPoints.size() + 2 * NUM_STEP)
     * Return:
     * - first: index (unsigned)
     * - second: t (double)
     */
    std::pair<unsigned,double> binSearchNearestPointOnSpline(const Ogre::Vector3& point) const;
};

}

#endif
