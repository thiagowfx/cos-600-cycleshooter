#ifndef _PATHMANAGER_HPP_
#define _PATHMANAGER_HPP_

#include <vector>

#include <OgreVector3.h>

namespace Cycleshooter {

class PathManager {

    std::vector<Ogre::Vector3> points;

public:
    PathManager();

    PathManager(const std::vector<Ogre::Vector3>& points);
};

}

#endif
