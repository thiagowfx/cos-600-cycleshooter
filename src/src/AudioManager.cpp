#include "AudioManager.hpp"

namespace Cycleshooter {

AudioManager &AudioManager::instance() {
    static AudioManager instance;
    return instance;
}

}

//sf::SoundBuffer buffer;
//if(!buffer.loadFromFile("../audio/shoot2.wav")) {
//    Ogre::LogManager::getSingleton().logMessage("damn");
//}
//sf::Sound sound;
//sound.setBuffer(buffer);
//sound.play();
