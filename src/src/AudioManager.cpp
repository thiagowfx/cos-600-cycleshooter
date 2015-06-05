#include "AudioManager.hpp"

namespace Cycleshooter {

AudioManager::AudioManager() {
    Ogre::LogManager::getSingleton().logMessage("--> AudioManager: Singleton Constructor <--");

    load_sounds();
    load_musics();
}

AudioManager &AudioManager::instance() {
    static AudioManager instance;
    return instance;
}

void AudioManager::play(Soundname soundname) {
    if(playing_sounds.size() == 0) {
        playing_sounds.push_back(sf::Sound());
        playing_sounds.at(0).setBuffer(sounds[soundname]);
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
            playing_sounds.at(location).setBuffer(sounds[soundname]);
            playing_sounds.at(location).play();
        }
        else {
            playing_sounds.push_back(sf::Sound());
            playing_sounds.back().setBuffer(sounds[soundname]);
            playing_sounds.back().play();
        }

    }
}

void AudioManager::play(Musicname musicname, bool restart) {
    sf::Lock lock(current_playing_music_mutex);

    Ogre::LogManager::getSingleton().logMessage("--> AudioManager: Play a new music <--");

    if(current_playing_music != NULL && current_playing_music->getStatus() == sf::Music::Playing) {
        current_playing_music->pause();
    }

    current_playing_music = &musics[musicname];

    if(restart) {
        current_playing_music->stop();
    }

    current_playing_music->play();
}

// TODO: fix this when toggling modes
void AudioManager::toggleMute() {
    if(current_playing_music) {
        current_playing_music->getVolume() == 0.0 ?  current_playing_music->setVolume(100.0) : current_playing_music->setVolume(0.0);
    }
}

void AudioManager::load_sounds() {
    Ogre::LogManager::getSingleton().logMessage("--> AudioManager: Loading Sounds <--");

    auto error = [](std::string soundname) {
        Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL, " |-> Error while loading a sound: " + soundname);
        exit(1);
    };

    if(!sounds[SOUND_SHOOT1].loadFromFile(AUDIO_PATH + "shoot1.wav"))
        error("shoot1.wav");

    if(!sounds[SOUND_SHOOT2].loadFromFile(AUDIO_PATH + "shoot2.wav"))
        error("shoot2.wav");

    if(!sounds[SOUND_SHOOT3].loadFromFile(AUDIO_PATH + "shoot3.wav"))
        error("shoot3.wav");
}

void AudioManager::load_musics() {
    Ogre::LogManager::getSingleton().logMessage("--> AudioManager: Loading Musics <--");

    auto error = [](std::string musicname) {
        Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL, " |-> Error while loading a music: " + musicname);
        exit(1);
    };

    if(!musics[MUSIC_RUNNER1_BFMV_HAND_OF_BLOOD].openFromFile(MUSIC_PATH + "bfmv-hand-of-blood.ogg"))
        error("bfmv-hand-of-blood.ogg");

    if(!musics[MUSIC_RUNNER2_DISTURBED_DECADENCE].openFromFile(MUSIC_PATH + "disturbed-decadence.ogg"))
        error("disturbed-decadence.ogg");

    if(!musics[MUSIC_RUNNER3_THREE_DAYS_GRACE_ANIMAL_I_HAVE_BECOME].openFromFile(MUSIC_PATH + "three-days-grace-animal-i-have-become.ogg"))
        error("three-days-grace-animal-i-have-become.ogg");

    if(!musics[MUSIC_SHOOTER1].openFromFile(MUSIC_PATH + "shooter1.ogg"))
        error("shooter1.ogg");
}

void AudioManager::random_play(const std::vector<Soundname>& sound_list) {
    play(sound_list[rand() % sound_list.size()]);
}

void AudioManager::random_play_shoot() {
    static std::vector<Soundname> sound_list = {SOUND_SHOOT1, SOUND_SHOOT2, SOUND_SHOOT3};
    random_play(sound_list);
}

}
