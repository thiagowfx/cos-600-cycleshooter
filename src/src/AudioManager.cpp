#include "AudioManager.hpp"

namespace Cycleshooter {

AudioManager::AudioManager() {
    Ogre::LogManager::getSingleton().logMessage("--> AudioManager: Constructor (Singleton) <--");

    populate_sounds();
    populate_musics();
}

AudioManager &AudioManager::instance() {
    static AudioManager instance;
    return instance;
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

void AudioManager::toggle_mute() {
    mute_music = !mute_music;
    do_mute();
}

void AudioManager::do_mute() {
    if(current_playing_music) {
        mute_music ? current_playing_music->setVolume(0.0) : current_playing_music->setVolume(100.0);
    }
}

void AudioManager::populate_sounds() {
    Ogre::LogManager::getSingleton().logMessage("--> AudioManager: Populate Sounds <--");

    auto error = [](const std::string& soundname) {
        Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL, "----> Error while loading a sound: " + soundname);
        exit(1);
    };

#define LOAD_TEMPLATE(soundname, soundfile)\
    do {\
        if(!sound_map[soundname].loadFromFile(SOUND_PATH + soundfile))\
            error(soundfile);\
    } while(0)

    LOAD_TEMPLATE(SOUND_SHOOT1, "shoot1.wav");
    LOAD_TEMPLATE(SOUND_SHOOT2, "shoot2.wav");
    LOAD_TEMPLATE(SOUND_SHOOT3, "shoot3.wav");

#undef LOAD_TEMPLATE
}

void AudioManager::populate_musics() {
    Ogre::LogManager::getSingleton().logMessage("--> AudioManager: Populate Musics <--");

    auto error = [](const std::string& musicname) {
        Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL, "----> Error while loading a music: " + musicname);
        exit(1);
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

    static std::vector<Soundname> shoot_sound_list = {SOUND_SHOOT1, SOUND_SHOOT2, SOUND_SHOOT3};
    play_random(shoot_sound_list);
}

}
