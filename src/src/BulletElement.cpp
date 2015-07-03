#include "BulletElement.hpp"

namespace Cycleshooter {


Ogre::String BulletElement::getScenenodeName() const{
    return scenenodeName;
}

void BulletElement::setScenenodeName(const Ogre::String &value){
    scenenodeName = value;
}

Ogre::Vector3 BulletElement::getCoordinate() const{
    return coordinate;
}

void BulletElement::setCoordinate(const Ogre::Vector3 &value){
    coordinate = value;
}

void BulletElement::setNewBullet(Ogre::Vector3 coord, int bulletNum){
    setCoordinate(coord);
    //Create bullet's scene node name base on the standard scene node name.
    std::ostringstream convert;
    convert << bulletNum;
    Ogre::String name = scenenodeName + convert.str();
    setScenenodeName(name);
}
BulletElement::BulletElement() {
    scenenodeName = "bulletSceneNode";
    coordinate = Ogre::Vector3::ZERO;
}

BulletElement::~BulletElement() {
}

}
