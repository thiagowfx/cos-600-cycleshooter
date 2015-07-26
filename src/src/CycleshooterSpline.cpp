#include "CycleshooterSpline.hpp"

namespace Cycleshooter {

CycleshooterSpline::CycleshooterSpline(std::vector<Ogre::Vector3>& points) {
    go(points);
}

CycleshooterSpline::CycleshooterSpline(const char *file) {
    std::ifstream ifs(file);
    std::string line;
    std::vector<Ogre::Vector3> points;

    while(std::getline(ifs, line)) {
        std::stringstream ss;
        ss << line;
        double x, y, z;
        ss >> x >> y >> z;
        points.push_back(Ogre::Vector3(x, y, z));
    }

    ifs.close();
    go(points);
}

Ogre::Vector3 CycleshooterSpline::getTangent(Ogre::Real coord) const {
    if((coord - EPSILON) < 0.0 || (coord + EPSILON) > path.getTotalLength()) {
        return path.getAvgDirection(0).normalisedCopy();
    }
    else {
        return (path.getPosition(coord + EPSILON) - path.getPosition(coord - EPSILON)).normalisedCopy();
    }
}

Ogre::Vector3 CycleshooterSpline::getTangent(unsigned int i, Ogre::Real coord) const {
    if((coord - EPSILON_TRACK) < 0.0) {
        return path.getAvgDirection(i).normalisedCopy();
    }
    else if((coord + EPSILON_TRACK) > 1.0) {
        return (path.getAvgDirection((i + 1) % path.getSegCount())).normalisedCopy();
    }
    else {
        return (path.getPosition(i, coord + EPSILON_TRACK) - path.getPosition(i, coord - EPSILON_TRACK)).normalisedCopy();
    }
}

Procedural::Path CycleshooterSpline::getPath() const {
    return path;
}

void CycleshooterSpline::go(std::vector<Ogre::Vector3> &points) {
    Procedural::RoundedCornerSpline3 spline;

    for(const auto& point: points) {
        spline.addPoint(point);
    }

    spline.close();

    path = spline.realizePath();
    path.close();
}

std::ostream& operator<<(std::ostream& os, const CycleshooterSpline& cs) {
    const auto points = cs.path.getPoints();

    for(const auto& point: points) {
        std::cout << point << std::endl;
    }

    return os;
}

}
