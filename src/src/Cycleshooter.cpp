#include "Controller.hpp"

int main(int argc, char *argv[]) {
    try {
        Cycleshooter::Controller(argc, argv);
    } catch(Ogre::Exception& e)  {
        std::cout << "An exception has occurred: " << e.getFullDescription() << std::endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}
