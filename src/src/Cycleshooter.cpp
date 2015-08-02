#include "AudioManager.hpp"
#include "ConfigManager.hpp"
#include "InputManager.hpp"

#include "Controller.hpp"

int main(int argc, char *argv[]) {
    try {
        // early initialization of *some* singletons, for performance purporses
        Cycleshooter::AudioManager::instance();
        Cycleshooter::ConfigManager::instance();
        Cycleshooter::InputManager::instance();

        // our application
        Cycleshooter::Controller(argc, argv);

    } catch(Ogre::Exception& e)  {
        std::cout << "An exception has occurred: " << e.getFullDescription() << std::endl;
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
