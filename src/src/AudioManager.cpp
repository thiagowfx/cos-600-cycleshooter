#include "AudioManager.hpp"

namespace Cycleshooter {

AudioManager::AudioManager() {
    load_sounds();
}

AudioManager &AudioManager::instance() {
    static AudioManager instance;
    return instance;
}

void AudioManager::play(Soundnames soundname) {
    if (playing_sounds.size() == 0) {
        playing_sounds.push_back(sf::Sound());
        playing_sounds.at(0).setBuffer(sounds[soundname]);
        playing_sounds.at(0).play();
    }

    else {
        for(std::deque<sf::Sound>::iterator it = playing_sounds.begin(); it != playing_sounds.end(); ++it) {
            if(it->getStatus() != sf::Sound::Playing)
                playing_sounds.erase(it);
        }

        playing_sounds.push_back(sf::Sound());
        playing_sounds.back().setBuffer(sounds[soundname]);
        playing_sounds.back().play();
    }
}

void AudioManager::load_sounds() {
    Ogre::LogManager::getSingleton().logMessage("AudioManager: Loading Sounds");

    sounds[SOUND_SHOOT1].loadFromFile(AUDIO_PATH + "shoot1.wav");
    sounds[SOUND_SHOOT2].loadFromFile(AUDIO_PATH + "shoot2.wav");
    sounds[SOUND_SHOOT3].loadFromFile(AUDIO_PATH + "shoot3.wav");
}

void AudioManager::random_play(std::vector<Soundnames> sound_list) {
    play(sound_list[rand() % sound_list.size()]);
}

void AudioManager::random_play_shoot() {
    random_play({
                   SOUND_SHOOT1,
                   SOUND_SHOOT2,
                   SOUND_SHOOT3
               });
}

}
