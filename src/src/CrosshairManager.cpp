#include "CrosshairManager.hpp"

namespace Cycleshooter {

void CrosshairManager::check_out_of_bounds(double &px, double &py, bool wraps) {
    if(px > 1.0) {
        px = wraps ? -1.0 : +1.0;
    }
    else if(px < -1.0) {
        px = wraps ? +1.0 : -1.0;
    }

    if(py > 1.0) {
        py = wraps ? -1.0 : +1.0;
    }
    else if(py < -1.0) {
        py = wraps ? +1.0 : -1.0;
    }
}

CrosshairManager::CrosshairManager() {
    crosshair = Ogre::OverlayManager::getSingleton().getByName("Cycleshooter/Crosshair");

    crosshair->setScale(CROSSHAIR_SCALE, CROSSHAIR_SCALE);
}

void CrosshairManager::scroll(const double x, const double y, bool wraps) {
    double px = crosshair->getScrollX();
    double py = crosshair->getScrollY();

    px += x;
    py += y;

    check_out_of_bounds(px, py, wraps);

    crosshair->setScroll(px, py);
}

std::pair<double, double> CrosshairManager::getScroll() const {
    return std::make_pair(getScrollX(), getScrollY());
}

double CrosshairManager::getScrollX() const {
    return crosshair->getScrollX();
}

double CrosshairManager::getScrollY() const {
    return crosshair->getScrollY();
}

void CrosshairManager::setupRunnerMode() {
    crosshair->hide();
}

void CrosshairManager::setupShooterMode() {
    crosshair->show();
}

}
