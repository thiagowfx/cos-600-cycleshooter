#ifndef _AUDIOMANAGER_HPP_
#define _AUDIOMANAGER_HPP_

#include <OgreLogManager.h>
#include <SFML/Audio.hpp>

namespace Cycleshooter {
/**
 * @brief The AudioManager class A singleton class to handle all game music and sounds.
 */
class AudioManager {
    // constructor and copy functions
    AudioManager(){}
    AudioManager(const AudioManager&) = delete;
    void operator=(const AudioManager&) = delete;

public:
    static AudioManager& instance();
};
}

#endif
