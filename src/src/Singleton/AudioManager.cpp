#include "AudioManager.hpp"

namespace Cycleshooter {

AudioManager::AudioManager() {
    Ogre::LogManager::getSingleton().logMessage("--> AudioManager: Constructor (Singleton) <--");

    populate_sounds();
    populate_musics();
}

void AudioManager::play_sound(const Soundname& soundname) {
    if(playing_sounds.size() == 0) {
        playing_sounds.push_back(sf::Sound());
        playing_sounds.at(0).setBuffer(sound_map[soundname]);
        playing_sounds.at(0).play();
    }

    else {
        int location = -1;

        for (int i = 0; i < playing_sounds.size(); ++i) {
            if (playing_sounds.at(i).getStatus() != sf::Sound::Playing && location == -1) {
                location = i;
            }
        }

        if (location != -1) {
            playing_sounds.at(location).setBuffer(sound_map[soundname]);
            playing_sounds.at(location).play();
        }
        else {
            playing_sounds.push_back(sf::Sound());
            playing_sounds.back().setBuffer(sound_map[soundname]);
            playing_sounds.back().play();
        }
    }
}

void AudioManager::play_music(const Musicname &musicname, bool restart) {
    sf::Lock lock(current_playing_music_mutex);
    Ogre::LogManager::getSingleton().logMessage("--> AudioManager: Play Music <--");

    if(current_playing_music != NULL && current_playing_music->getStatus() == sf::Music::Playing) {
        current_playing_music->pause();
    }

    current_playing_music = &music_map[musicname];
    do_mute();

    if(restart) {
        current_playing_music->stop();
    }
    current_playing_music->play();
}

void AudioManager::stop_music() {
    if(current_playing_music) {
        current_playing_music->stop();
    }
}

void AudioManager::toggle_mute() {
    mute_music = !mute_music;
    do_mute();
}

void AudioManager::do_mute() {
    if(current_playing_music) {
        mute_music ? current_playing_music->setVolume(0.0) : current_playing_music->setVolume(MUSIC_VOLUME_RATIO * 100.0);
    }
}

void AudioManager::populate_sounds() {
    Ogre::LogManager::getSingleton().logMessage("--> AudioManager: Populate Sounds <--");

    auto error = [](const std::string& soundname) {
        Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL, "----> AudioManager: Error while loading this sound: " + soundname);
        exit(EXIT_FAILURE);
    };

#define LOAD_TEMPLATE(soundname, soundfile)\
    do {\
        if(!sound_map[soundname].loadFromFile(SOUND_PATH + soundfile))\
            error(soundfile);\
    } while(0)

    LOAD_TEMPLATE(SOUND_SHOOT01, "shoot/shoot01.wav");
    LOAD_TEMPLATE(SOUND_SHOOT02, "shoot/shoot02.wav");
    LOAD_TEMPLATE(SOUND_SHOOT03, "shoot/shoot03.wav");
    LOAD_TEMPLATE(SOUND_SHOOT04, "shoot/shoot04.wav");

    LOAD_TEMPLATE(SOUND_DRY_SHOOT, "shoot/dry_shoot.wav");

    LOAD_TEMPLATE(SOUND_GAME_LOSS, "events/game_loss.wav");
    LOAD_TEMPLATE(SOUND_GAME_VICTORY, "events/game_victory.wav");
    LOAD_TEMPLATE(SOUND_MONSTER_HIT, "events/monster_hit.wav");

    LOAD_TEMPLATE(SOUND_HEARTBEAT01, "heartbeat/heartbeat01.wav");
    LOAD_TEMPLATE(SOUND_HEARTBEAT02, "heartbeat/heartbeat02.wav");
    LOAD_TEMPLATE(SOUND_HEARTBEAT03, "heartbeat/heartbeat03.wav");
    LOAD_TEMPLATE(SOUND_HEARTBEAT04, "heartbeat/heartbeat04.wav");
    LOAD_TEMPLATE(SOUND_HEARTBEAT05, "heartbeat/heartbeat05.wav");

#undef LOAD_TEMPLATE
}

void AudioManager::populate_musics() {
    Ogre::LogManager::getSingleton().logMessage("--> AudioManager: Populate Musics <--");

    auto error = [](const std::string& musicname) {
        Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL, "----> AudioManager: Error while loading this music: " + musicname);
        exit(EXIT_FAILURE);
    };

#define LOAD_TEMPLATE(musicname, musicfile)\
    do {\
        if(!music_map[musicname].openFromFile(MUSIC_PATH + musicfile))\
            error(musicfile);\
    } while(0)

    LOAD_TEMPLATE(MUSIC_RUNNER, "bfmv-hand-of-blood.ogg");
    LOAD_TEMPLATE(MUSIC_SHOOTER, "hiroshi-okubo-arcade-ripping-air.ogg");

#undef LOAD_TEMPLATE
}

void AudioManager::play_random(const std::vector<Soundname>& sound_list) {
    play_sound(sound_list[rand() % sound_list.size()]);
}

void AudioManager::play_random_shoot() {
    Ogre::LogManager::getSingleton().logMessage("--> AudioManager: Play Random Shoot <--");

    static std::vector<Soundname> shoot_sound_list = {SOUND_SHOOT01, SOUND_SHOOT02, SOUND_SHOOT03, SOUND_SHOOT04};
    play_random(shoot_sound_list);
}

Soundname AudioManager::get_soundname_from_heartbeat(int level, int minimum, int maximum) {
    static std::vector<Soundname> heartbeat_sound_list = {SOUND_HEARTBEAT01, SOUND_HEARTBEAT02, SOUND_HEARTBEAT03, SOUND_HEARTBEAT04, SOUND_HEARTBEAT05};
    static const int range_size = (maximum - minimum) / heartbeat_sound_list.size();

    // clamping
    level = std::min(maximum, level);
    level = std::max(minimum, level);

    int sound = (level - minimum) / range_size;

    switch(sound) {
    case 0:
        return SOUND_HEARTBEAT01;
    case 1:
        return SOUND_HEARTBEAT02;
    case 2:
        return SOUND_HEARTBEAT03;
    case 3:
        return SOUND_HEARTBEAT04;
    case 4:
    default:
        return SOUND_HEARTBEAT05;
    }
}

void AudioManager::play_heartbeat(int level, int minimum, int maximum) {
    Soundname soundname = get_soundname_from_heartbeat(level, minimum, maximum);
    heartbeat_sound.setBuffer(sound_map[soundname]);
    heartbeat_sound.play();
}

sf::Time AudioManager::get_sound_duration(const Soundname &soundname) {
    return sound_map[soundname].getDuration();
}

}
