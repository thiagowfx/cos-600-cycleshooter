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

void AudioManager::specialAddSound(const Soundname &soundname, bool loops) {
    sf::Sound sound;
    sound.setBuffer(soundMap[soundname]);
    sound.setLoop(loops);
    specialSounds[soundname] = sound;
}

void AudioManager::specialRemoveSound(const Soundname &soundname) {
    specialSounds[soundname].stop();
    specialSounds.erase(soundname);
}

sf::Sound* AudioManager::specialGetSound(const Soundname &soundname) {
    return &specialSounds[soundname];
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
        LOG_FATAL("Couldn't load the %s sound", soundname.c_str());
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
    LOAD_TEMPLATE(SOUND_GAME_BATTLE_PREPARE_DOTA, "events/game_start_battle_prepare_dota.wav");
    LOAD_TEMPLATE(SOUND_GAME_BATTLE_BEGINS_DOTA, "events/game_start_battle_begins_dota.wav");
    LOAD_TEMPLATE(SOUND_MONSTER_CHASING, "events/monster_chasing.wav");
    LOAD_TEMPLATE(SOUND_MONSTER_HIT, "events/monster_hit.wav");
    LOAD_TEMPLATE(SOUND_MONSTER_DEATH, "events/monster_death.wav");
    LOAD_TEMPLATE(SOUND_MONSTER_DEATH_DOTA, "events/monster_death_dota.wav");
    LOAD_TEMPLATE(SOUND_PLAYER_DEATH, "events/playerdeath.wav");
    LOAD_TEMPLATE(SOUND_WALL_CRASH, "events/wall_crash.wav");
    LOAD_TEMPLATE(SOUND_WALL_CRASH, "events/reload_gun.wav");

    LOAD_TEMPLATE(SOUND_HEARTBEAT01, "heartbeat/heartbeat01.wav");
    LOAD_TEMPLATE(SOUND_HEARTBEAT02, "heartbeat/heartbeat02.wav");
    LOAD_TEMPLATE(SOUND_HEARTBEAT03, "heartbeat/heartbeat03.wav");
    LOAD_TEMPLATE(SOUND_HEARTBEAT04, "heartbeat/heartbeat04.wav");
    LOAD_TEMPLATE(SOUND_HEARTBEAT05, "heartbeat/heartbeat05.wav");

    LOAD_TEMPLATE(SOUND_COUNTDOWN, "events/countdown_beep.wav");

#undef LOAD_TEMPLATE
}

void AudioManager::populateMusics() {
    LOG("Populating musics");

    auto error = [](const std::string& musicname) {
        LOG_FATAL("Couldn't load the %s music", musicname.c_str());
        exit(EXIT_FAILURE);
    };

#define LOAD_TEMPLATE(musicname, musicfile)\
    do {\
        if(!musicMap[musicname].openFromFile(MUSIC_PATH + musicfile))\
            error(musicfile);\
    } while(0)

    LOAD_TEMPLATE(MUSIC_RUNNER, ConfigManager::instance().getStr("AudioManager.music_runner"));
    LOAD_TEMPLATE(MUSIC_SHOOTER, ConfigManager::instance().getStr("AudioManager.music_shooter"));

#undef LOAD_TEMPLATE
}

void AudioManager::playRandom(const std::vector<Soundname>& soundList) {
    playSound(soundList[rand() % soundList.size()]);
}

void AudioManager::playRandomShoot() {
    static std::vector<Soundname> shootSoundList = {SOUND_SHOOT01, SOUND_SHOOT02, SOUND_SHOOT03, SOUND_SHOOT04};
    playRandom(shootSoundList);
}

Soundname AudioManager::getSoundnameFromHeartbeat(int level, int minimum, int maximum) {
    static std::vector<Soundname> heartbeatSoundList = {SOUND_HEARTBEAT01, SOUND_HEARTBEAT02, SOUND_HEARTBEAT03, SOUND_HEARTBEAT04, SOUND_HEARTBEAT05};
    static const int rangeSize = (maximum - minimum) / heartbeatSoundList.size();

    // clamping
    level = std::min(maximum, level);
    level = std::max(minimum, level);

    int sound = (level - minimum) / rangeSize;

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
