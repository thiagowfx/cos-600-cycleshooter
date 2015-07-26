#ifndef _CYCLESHOOTERSPLINE_HPP_
#define _CYCLESHOOTERSPLINE_HPP_

#include <fstream>

#include <ProceduralPathGenerators.h>

namespace Cycleshooter {

class CycleshooterSpline {

    Procedural::Path path;

    void go(std::vector<Ogre::Vector3>& points);

public:
    CycleshooterSpline(std::vector<Ogre::Vector3>& points);
    CycleshooterSpline(const char* file);

    friend std::ostream& operator<<(std::ostream& os, const CycleshooterSpline& cs);
};

std::ostream& operator<<(std::ostream& os, const CycleshooterSpline& cs);

}

#endif
