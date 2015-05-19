#ifndef _ABSTRACTPOLAR_HPP_
#define _ABSTRACTPOLAR_HPP_

namespace Cycleshooter {
class AbstractPolar {
public:
    // TODO: change short -> unsigned
    virtual int getInstantaneousHeartRate() = 0;
    virtual int getMeanHeartRate() = 0;
};
}

#endif
