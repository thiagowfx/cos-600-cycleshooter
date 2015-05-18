#ifndef _AbstractPolar_HPP_
#define _AbstractPolar_HPP_

namespace Cycleshooter {
class AbstractPolar {
public:
    AbstractPolar();
    virtual ~AbstractPolar();
    virtual unsigned short readInstantaneousHeartRate() = 0;
    virtual unsigned short readMeanHeartRate() = 0;
};
}

#endif
