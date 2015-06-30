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
    // shoot (weapon fire) sounds
    SOUND_SHOOT01,
    SOUND_SHOOT02,
    SOUND_SHOOT03,
    SOUND_SHOOT04,


    // dry shoot (=no ammo left)
    SOUND_DRY_SHOOT,

    // game events
    SOUND_GAME_LOSS,
    SOUND_GAME_VICTORY,

    // heartbeat sounds
    SOUND_HEARTBEAT01,
    SOUND_HEARTBEAT02,
    SOUND_HEARTBEAT03,
    SOUND_HEARTBEAT04,
    SOUND_HEARTBEAT05,
};

/**
 * Each element represents an unique music.
 */
enum Musicname {
    // runner mode music
    MUSIC_RUNNER,

    // shooter mode music
    MUSIC_SHOOTER,
};

/**
 * @brief The AudioManager class A singleton class to handle all game music and sounds.
 *
 * It also acts as a SoundManager. Inspired originally by: http://stackoverflow.com/a/27860198
 * Note: sound = short duration; music = long duration; audio = generic word to refer to these ones.
 * Only one background music is allowed at any time. However, you may play several sound effects simultaneously.
 */
class AudioManager {
    // constructor and copy functions to make this class a singleton
    AudioManager();
    AudioManager(const AudioManager&) = delete;
    void operator=(const AudioManager&) = delete;

public:
    static AudioManager& instance() {
        static AudioManager instance;
        return instance;
    }

private:
    /**
     * @brief AUDIO_PATH Location of the sounds. It must have a trailing slash.
     */
    const std::string SOUND_PATH = "../audio/sounds/";

    /**
     * @brief MUSIC_PATH Location of the musics. It must have a trailing slash.
     */
    const std::string MUSIC_PATH = "../audio/music/";

    /**
     * @brief SOUND_MUSIC_VOLUME_RATIO How much is the music volume lower than the sound volume, relatively?
     * E.g. 0.5 means the music is 50% lower than the sound.
     */
    const double MUSIC_VOLUME_RATIO = 0.33;

    /**
     * Maps all soundnames to their respective soundbuffers.
     */
    std::map<Soundname, sf::SoundBuffer> sound_map;

    /**
     * Maps all musicnames to their respective Music variables.
     */
    std::map<Musicname, sf::Music> music_map;

    /**
     * Populate the sounds map.
     */
    void populate_sounds();

    /**
     * Populate the musics map.
     */
    void populate_musics();

    /**
     * Manages sounds being played currently.
     */
    std::deque<sf::Sound> playing_sounds;

    /**
     * The current playing music, if any.
     */
    sf::Music* current_playing_music = NULL;

    /**
     * Prevents simultaneous access to the current_playing_music variable.
     */
    sf::Mutex current_playing_music_mutex;

    /**
     * Randomly plays a sound from the specified sound list.
     */
    void play_random(const std::vector<Soundname>& sound_list);

    /**
     * Is the music muted?
     */
    bool mute_music = false;

    /**
     * Mute the current playing music if the mute_music variable is set to true.
     */
    void do_mute();

    /**
     * Is a heartbeat sound playing currently?
     */
    sf::Sound heartbeat_sound;

    /**
     * Map a heartbeat level to a heartbeat sound.
     */
    Soundname get_soundname_from_heartbeat(int level, int minimum, int maximum);

public:
    /**
     * Play the sound specified as argument.
     */
    void play_sound(const Soundname& soundname);

    /**
     * Play the music specified as argument, stopping the current one·
     * If restart is false, then it resumes the playback from the latest point where it stopped.
     */
    void play_music(const Musicname& musicname, bool restart = false);

    /**
     * Stop the game music.
     */
    void stop_music();

    /**
     * Toggle the mute status of the music.
     */
    void toggle_mute();

    /**
     * Play a random shoot (weapon fire) sound.
     */
    void play_random_shoot();

    /**
     *  Play a heartbeat sound, according to the specified level.
     */
    void play_heartbeat(int level, int minimum, int maximum);
};

}

#endif
