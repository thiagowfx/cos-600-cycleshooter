#include "Controller.hpp"

#ifdef __cplusplus
extern "C" {
#endif

// TODO: use argc and argv
int main(int argc, char *argv[]) {
    try {
        Cycleshooter::Controller* app  = new Cycleshooter::Controller();
        delete app;
    } catch(Ogre::Exception& e)  {
        std::cerr << "An exception has occurred: " <<
                     e.getFullDescription().c_str() << std::endl;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
