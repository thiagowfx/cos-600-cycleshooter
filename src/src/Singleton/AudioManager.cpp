#include "AudioManager.hpp"

namespace Cycleshooter {

AudioManager::AudioManager() {
    populateSounds();
    populateMusics();
}

void AudioManager::playSound(const Soundname& soundname) {
    if(playingSounds.size() == 0) {
        playingSounds.push_back(sf::Sound());
        playingSounds.at(0).setBuffer(soundMap[soundname]);
        playingSounds.at(0).play();
    }

    else {
        int location = -1;

        for (int i = 0; i < playingSounds.size(); ++i) {
            if (playingSounds.at(i).getStatus() != sf::Sound::Playing && location == -1) {
                location = i;
            }
        }

        if (location != -1) {
            playingSounds.at(location).setBuffer(soundMap[soundname]);
            playingSounds.at(location).play();
        }
        else {
            playingSounds.push_back(sf::Sound());
            playingSounds.back().setBuffer(soundMap[soundname]);
            playingSounds.back().play();
        }
    }
}

void AudioManager::playMusic(const Musicname &musicname, bool restart) {
    sf::Lock lock(currentPlayingMusicMutex);
    LOG("Playing Music");

    if(currentPlayingMusic != NULL && currentPlayingMusic->getStatus() == sf::Music::Playing) {
        currentPlayingMusic->pause();
    }

    currentPlayingMusic = &musicMap[musicname];
    doMute();

    if(restart) {
        currentPlayingMusic->stop();
    }
    currentPlayingMusic->play();
}

void AudioManager::stopMusic() {
    if(currentPlayingMusic) {
        currentPlayingMusic->stop();
    }
}

void AudioManager::toggleMute() {
    LOG("Toggling mute");
    musicMuted = !musicMuted;
    doMute();
}

void AudioManager::doMute() {
    if(currentPlayingMusic) {
        musicMuted ? currentPlayingMusic->setVolume(0.0) : currentPlayingMusic->setVolume(MUSIC_VOLUME_RATIO * 100.0);
    }
}

void AudioManager::populateSounds() {
    LOG("Populating sounds");

    auto error = [](const std::string& soundname) {
        LOG_FATAL("Couldn't load the %s sound", soundname);
        exit(EXIT_FAILURE);
    };

#define LOAD_TEMPLATE(soundname, soundfile)\
    do {\
        if(!soundMap[soundname].loadFromFile(SOUND_PATH + soundfile))\
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

void AudioManager::populateMusics() {
    LOG("Populating musics");

    auto error = [](const std::string& musicname) {
        LOG_FATAL("Couldn't load the %s music", musicname);
        exit(EXIT_FAILURE);
    };

#define LOAD_TEMPLATE(musicname, musicfile)\
    do {\
        if(!musicMap[musicname].openFromFile(MUSIC_PATH + musicfile))\
            error(musicfile);\
    } while(0)

    LOAD_TEMPLATE(MUSIC_RUNNER, "bfmv-hand-of-blood.ogg");
    LOAD_TEMPLATE(MUSIC_SHOOTER, "hiroshi-okubo-arcade-ripping-air.ogg");

#undef LOAD_TEMPLATE
}

void AudioManager::playRandom(const std::vector<Soundname>& sound_list) {
    playSound(sound_list[rand() % sound_list.size()]);
}

void AudioManager::playRandomShoot() {
    static std::vector<Soundname> shoot_sound_list = {SOUND_SHOOT01, SOUND_SHOOT02, SOUND_SHOOT03, SOUND_SHOOT04};
    playRandom(shoot_sound_list);
}

Soundname AudioManager::getSoundnameFromHeartbeat(int level, int minimum, int maximum) {
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

void AudioManager::playHeartbeat(int level, int minimum, int maximum) {
    Soundname soundname = getSoundnameFromHeartbeat(level, minimum, maximum);
    heartbeatSound.setBuffer(soundMap[soundname]);
    heartbeatSound.play();
}

sf::Time AudioManager::getSoundDuration(const Soundname &soundname) const {
    return soundMap.at(soundname).getDuration();
}

}
