#ifndef _PATHMANAGER_HPP_
#define _PATHMANAGER_HPP_

#include <vector>

#include <OgreVector3.h>
#include <OgreSimpleSpline.h>

namespace Cycleshooter {

class PathManager: public Ogre::SimpleSpline {

    Ogre::Vector3 currentTangent;
    Ogre::Vector3 lastTangent;
    Ogre::Real t; //parametric portion of the whole Spline curve starts on 0.003 and goes to 1
    Ogre::Real increment = 0.0001;

public:
    PathManager();

    void updateTangents();
    void updateT();

    Ogre::Vector3 getCurrentTangent() const;
    Ogre::Vector3 getLastTangent() const;
};

}

#endif
