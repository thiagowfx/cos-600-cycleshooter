#ifndef _AUDIOMANAGER_HPP_
#define _AUDIOMANAGER_HPP_

#include <deque>
#include <map>

#include <OgreLogManager.h>
#include <SFML/Audio.hpp>

namespace Cycleshooter {

/**
 * Each element represents a unique sound.
 */
enum Soundname {
    // shoot (fire weapon) sounds
    SOUND_SHOOT1,
    SOUND_SHOOT2,
    SOUND_SHOOT3,
};

/**
 * @brief The AudioManager class A singleton class to handle all game music and sounds.
 * It also acts as a SoundManager. Inspired originally by: http://stackoverflow.com/a/27860198
 */
class AudioManager {
    // constructor and copy functions
    AudioManager();
    AudioManager(const AudioManager&) = delete;
    void operator=(const AudioManager&) = delete;

    /**
     * @brief AUDIO_PATH Location of the sounds.
     */
    const std::string AUDIO_PATH = "../audio/";

    /**
     * Maps all soundnames to their respective soundbuffers.
     */
    std::map<Soundname, sf::SoundBuffer> sounds;

    /**
     * Manages currently playing sounds.
     */
    std::deque<sf::Sound> playing_sounds;

    /**
     * Populate the sounds map.
     */
    void load_sounds();

    /**
     * Randomly plays a sound from the specified list.
     */
    void random_play(const std::vector<Soundname>& sound_list);

public:
    static AudioManager& instance();

    /**
     * Play the sound specified as argument.
     */
    void play(Soundname soundname);

    /**
     * Play a random shoot (fire) sound.
     */
    void random_play_shoot();
};
}

#endif
