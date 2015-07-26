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
