#include "PathManager.hpp"

namespace Cycleshooter {

PathManager::PathManager() {
}

PathManager::PathManager(const std::vector<Ogre::Vector3> &points) :
    points(points)
{
}

}
