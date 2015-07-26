#ifndef _CYCLESHOOTERSPLINE_HPP_
#define _CYCLESHOOTERSPLINE_HPP_

#include <fstream>

#include <ProceduralPathGenerators.h>

namespace Cycleshooter {

class CycleshooterSpline {

    // relative to 1.0
    const double EPSILON = 1e-3;

    const double EPSILON_TRACK = 1.0;

    Procedural::Path path;

    void go(std::vector<Ogre::Vector3>& points);

public:
    CycleshooterSpline(std::vector<Ogre::Vector3>& points);
    CycleshooterSpline(const char* file);
    Ogre::Vector3 getTangent(Ogre::Real coord) const;
    Ogre::Vector3 getTangent(unsigned int i, Ogre::Real coord) const;

    friend std::ostream& operator<<(std::ostream& os, const CycleshooterSpline& cs);
    Procedural::Path getPath() const;
};

std::ostream& operator<<(std::ostream& os, const CycleshooterSpline& cs);

}

#endif
