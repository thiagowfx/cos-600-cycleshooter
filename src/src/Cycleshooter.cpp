#include <cstring>
#include "Controller.hpp"

int main(int argc, char *argv[]) {
    try {
        if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
            std::cout << argv[0] << " [<fullscreen> [<width> <height>]]" << std::endl;
            std::cout << "Examples:" << std::endl;
            std::cout << "\t" << argv[0] << " 0 1366 768" << std::endl;
            std::cout << "\t" << argv[0] << " 1 800 600" << std::endl;
        }
        else {
            Cycleshooter::Controller(argc, argv);
        }
    } catch(Ogre::Exception& e)  {
        std::cerr << "An exception has occurred: " << e.getFullDescription().c_str() << std::endl;
    }

    return 0;
}
