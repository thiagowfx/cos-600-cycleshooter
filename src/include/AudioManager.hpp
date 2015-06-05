#ifndef _AUDIOMANAGER_HPP_
#define _AUDIOMANAGER_HPP_

#include <deque>
#include <map>

#include <OgreLogManager.h>
#include <SFML/Audio.hpp>
#include <SFML/System/Mutex.hpp>

namespace Cycleshooter {

/**
 * Each element represents an unique sound.
 */
enum Soundname {
    // shoot (fire weapon) sounds
    SOUND_SHOOT1,
    SOUND_SHOOT2,
    SOUND_SHOOT3,
};

/**
 * Each element represents an unique music.
 */
enum Musicname {
    // runner mode music
    MUSIC_RUNNER1,

    // shooter mode music
    MUSIC_SHOOTER1,
};

/**
 * @brief The AudioManager class A singleton class to handle all game music and sounds.
 * It also acts as a SoundManager. Inspired originally by: http://stackoverflow.com/a/27860198
 * Note: sound = short duration; music = long duration; audio = generic word to refer to these.
 * Only one background music is allowed at once. However, you may play several sounds simultaneously.
 */
class AudioManager {
    // constructor and copy functions
    AudioManager();
    AudioManager(const AudioManager&) = delete;
    void operator=(const AudioManager&) = delete;

    /**
     * @brief AUDIO_PATH Location of the sounds. It should have a trailing slash.
     */
    const std::string AUDIO_PATH = "../audio/sounds/";

    /**
     * @brief MUSIC_PATH Location of the musics. It should have a trailing slash.
     */
    const std::string MUSIC_PATH = "../audio/music/";

    /**
     * Maps all soundnames to their respective soundbuffers.
     */
    std::map<Soundname, sf::SoundBuffer> sounds;

    /**
     * Maps all musicnames to their respective Music variables.
     */
    std::map<Musicname, sf::Music> musics;

    /**
     * Manages currently playing sounds.
     */
    std::deque<sf::Sound> playing_sounds;

    /**
     * Populate the sounds map.
     */
    void load_sounds();

    /**
     * Populate the musics map.
     */
    void load_musics();

    /**
     * Randomly plays a sound from the specified list.
     */
    void random_play(const std::vector<Soundname>& sound_list);

    /**
     * @brief current_playing_music_mutex Prevents simultaneous access to the current_playing_music variable.
     */
    sf::Mutex current_playing_music_mutex;

    /**
     * The current playing music, if any.
     */
    sf::Music* current_playing_music = NULL;

public:
    static AudioManager& instance();

    /**
     * Play the sound specified as argument.
     */
    void play(Soundname soundname);

    /**
     * Stops playing the current music, and begins to play the one passed as argument.
     * If restart is false, the music will keep playing from where it stopped before, otherwise it will begin from its start.
     */
    void play(Musicname musicname, bool restart = false);

    /**
     * Mute the current playing music.
     */
    void toggleMute();

    /**
     * Play a random shoot (fire) sound.
     */
    void random_play_shoot();
};
}

#endif
