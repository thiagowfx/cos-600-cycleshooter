#ifndef _TUTORIALAPPLICATION_HPP_
#define _TUTORIALAPPLICATION_HPP_

#include "BaseApplication.hpp"

class TutorialApplication : public BaseApplication
{
public:
    TutorialApplication();
    virtual ~TutorialApplication();

protected:
    virtual void createScene();
};


#endif
