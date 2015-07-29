#ifndef _BULLETELEMENT_HPP_
#define _BULLETELEMENT_HPP_

#include <OgreVector3.h>
#include<OgreString.h>

namespace Cycleshooter {
class BulletElement {

private:
    Ogre::Vector3 coordinate;
    Ogre::String scenenodeName;
public:
    BulletElement();
    virtual ~BulletElement();

    Ogre::String getScenenodeName() const;
    void setScenenodeName(const Ogre::String &value);
    Ogre::Vector3 getCoordinate() const;
    void setCoordinate(const Ogre::Vector3 &value);
    std::pair<Ogre::Vector3,Ogre::String> getProperties();
    void setNewBullet(Ogre::Vector3 coord, int bulletNum);
};
}

#endif
