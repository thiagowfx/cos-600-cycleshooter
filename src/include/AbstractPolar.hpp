#ifndef _AbstractPolar_HPP_
#define _AbstractPolar_HPP_

namespace Cycleshooter {
class AbstractPolar {
public:
    AbstractPolar();
    virtual ~AbstractPolar();
    virtual short readInstantaneousHeartRate() = 0;
    virtual short readMeanHeartRate() = 0;
};
}

#endif
