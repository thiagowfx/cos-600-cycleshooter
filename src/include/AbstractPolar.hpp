#ifndef _ABSTRACTPOLAR_HPP_
#define _ABSTRACTPOLAR_HPP_

namespace Cycleshooter {
class AbstractPolar {
public:
    virtual short readInstantaneousHeartRate() = 0;
    virtual short readMeanHeartRate() = 0;
};
}

#endif
