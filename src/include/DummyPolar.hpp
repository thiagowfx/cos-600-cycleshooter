#ifndef _DummyPolar_HPP_
#define _DummyPolar_HPP_

#include "AbstractPolar.hpp"

namespace Cycleshooter {
class DummyPolar : public AbstractPolar {

public:
    DummyPolar();
    virtual ~DummyPolar();
    short readInstantaneousHeartRate();
    short readMeanHeartRate();
};

}

#endif
