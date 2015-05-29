#include "InputManager.hpp"

namespace Cycleshooter {

bool InputManager::hasKey(const sf::Keyboard::Key &key, const Context &mode) {
    switch(mode) {
    case CONTEXT_RUNNER:
        return runnerKeyboardMap.find(key) != runnerKeyboardMap.end();
        break;
    case CONTEXT_SHOOTER:
        return shooterKeyboardMap.find(key) != shooterKeyboardMap.end();
        break;
    }
}

bool InputManager::hasJoystickKey(const sf::Keyboard::Key &key, const Context &mode){
    switch(mode) {
    case CONTEXT_RUNNER:
        return runnerJoystickKeyboardMap.find(key) != runnerJoystickKeyboardMap.end();
        break;
    case CONTEXT_SHOOTER:
        return shooterJoystickKeyboardMap.find(key) != shooterJoystickKeyboardMap.end();
        break;
    }
}

bool InputManager::hasKeyUnbuf(const sf::Keyboard::Key &key, const Context &mode) {
    switch(mode) {
    case CONTEXT_RUNNER:
        return uRunnerKeyboardMap.find(key) != uRunnerKeyboardMap.end();
        break;
    case CONTEXT_SHOOTER:
        return uShooterKeyboardMap.find(key) != uShooterKeyboardMap.end();
        break;
    }
}

bool InputManager::hasAxisUnbuf(const sf::Joystick::Axis& axis, const Context &mode) {
    switch(mode) {
    case CONTEXT_RUNNER:
        return uRunnerJoystickAxisMap.find(axis) != uRunnerJoystickAxisMap.end();
        break;
    case CONTEXT_SHOOTER:
        return uShooterJoystickAxisMap.find(axis) != uShooterJoystickAxisMap.end();
        break;
    }
}

InputManager &InputManager::instance() {
    // guaranteed to be destroyed
    static InputManager instance;

    // instantiated on first use
    return instance;
}

void InputManager::addKey(const sf::Keyboard::Key &key, const std::function<void ()> &action) {
    addKey(key, CONTEXT_RUNNER, action);
    addKey(key, CONTEXT_SHOOTER, action);
}

void InputManager::addJoystickKey(const sf::Keyboard::Key &key, const std::function<void ()> &action){
    addJoystickKey(key, CONTEXT_RUNNER, action);
    addJoystickKey(key, CONTEXT_SHOOTER, action);
}

void InputManager::addKeyUnbuf(const sf::Keyboard::Key &key, const std::function<void ()> &action) {
    addKeyUnbuf(key, CONTEXT_RUNNER, action);
    addKeyUnbuf(key, CONTEXT_SHOOTER, action);
}

void InputManager::addAxisUnbuf(const sf::Joystick::Axis &axis, const std::function<void (float)> &action) {
    addAxisUnbuf(axis, CONTEXT_RUNNER, action);
    addAxisUnbuf(axis, CONTEXT_SHOOTER, action);
}

void InputManager::addJoystickKey(const sf::Keyboard::Key &key, const Context &mode, const std::function<void ()> &action){
    switch(mode) {
    case CONTEXT_RUNNER:
        runnerJoystickKeyboardMap[key] = action;
        break;
    case CONTEXT_SHOOTER:
        shooterJoystickKeyboardMap[key] = action;
        break;
    }
}

void InputManager::addKey(const sf::Keyboard::Key &key, const Context &mode, const std::function<void ()> &action) {
    switch(mode) {
    case CONTEXT_RUNNER:
        runnerKeyboardMap[key] = action;
        break;
    case CONTEXT_SHOOTER:
        shooterKeyboardMap[key] = action;
        break;
    }
}

void InputManager::addKeyUnbuf(const sf::Keyboard::Key &key, const Context &mode, const std::function<void ()> &action) {
    switch(mode) {
    case CONTEXT_RUNNER:
        uRunnerKeyboardMap[key] = action;
        break;
    case CONTEXT_SHOOTER:
        uShooterKeyboardMap[key] = action;
        break;
    }
}

void InputManager::addAxisUnbuf(const sf::Joystick::Axis &axis, const Context &mode, const std::function<void (float)> &action) {
    switch(mode) {
    case CONTEXT_RUNNER:
        uRunnerJoystickAxisMap[axis] = action;
        break;
    case CONTEXT_SHOOTER:
        uShooterJoystickAxisMap[axis] = action;
        break;
    }
}

void InputManager::addKeys(const std::vector<sf::Keyboard::Key> &keys, const std::function<void ()> &action) {
    for(const auto& key: keys) {
        addKey(key, action);
    }
}

void InputManager::addJoystickKeys(const std::vector<sf::Keyboard::Key> &keys, const std::function<void ()> &action){
    for(const auto& key: keys) {
        addJoystickKey(key, action);
    }
}

void InputManager::addKeysUnbuf(const std::vector<sf::Keyboard::Key> &keys, const std::function<void ()> &action) {
    for(const auto& key: keys) {
        addKeyUnbuf(key, action);
    }
}

void InputManager::addKeys(const std::vector<sf::Keyboard::Key> &keys, const Context &mode, const std::function<void ()> &action) {
    for(const auto& key: keys) {
        addKey(key, mode, action);
    }
}

void InputManager::addJoystickKeys(const std::vector<sf::Keyboard::Key> &keys, const Context &mode, const std::function<void ()> &action){
    for(const auto& key: keys) {
        addJoystickKey(key, mode, action);
    }
}

void InputManager::addKeysUnbuf(const std::vector<sf::Keyboard::Key> &keys, const Context &mode, const std::function<void ()> &action) {
    for(const auto& key: keys) {
        addKeyUnbuf(key, mode, action);
    }
}

void InputManager::removeKey(const sf::Keyboard::Key &key, const Context &mode) {
    if(!hasKey(key, mode))
        return;

    switch(mode) {
    case CONTEXT_RUNNER:
        runnerKeyboardMap.erase(key);
        break;
    case CONTEXT_SHOOTER:
        shooterKeyboardMap.erase(key);
        break;
    }
}

void InputManager::removeJoystickKey(const sf::Keyboard::Key &key, const Context &mode){
    if(!hasKey(key, mode))
        return;

    switch(mode) {
    case CONTEXT_RUNNER:
        runnerJoystickKeyboardMap.erase(key);
        break;
    case CONTEXT_SHOOTER:
        shooterJoystickKeyboardMap.erase(key);
        break;
    }
}

void InputManager::removeKeyUnbuf(const sf::Keyboard::Key &key, const Context &mode) {
    if(!hasKeyUnbuf(key, mode))
        return;

    switch(mode) {
    case CONTEXT_RUNNER:
        uRunnerKeyboardMap.erase(key);
        break;
    case CONTEXT_SHOOTER:
        uShooterKeyboardMap.erase(key);
        break;
    }
}

void InputManager::removeAxisUnbuf(const sf::Joystick::Axis &axis, const Context &mode) {
    if(!hasAxisUnbuf(axis, mode))
        return;

    switch(mode) {
    case CONTEXT_RUNNER:
        uRunnerJoystickAxisMap.erase(axis);
        break;
    case CONTEXT_SHOOTER:
        uShooterJoystickAxisMap.erase(axis);
        break;
    }
}

void InputManager::removeKeys(const std::vector<sf::Keyboard::Key> &keys, const Context &mode) {
    for(const auto& key: keys) {
        removeKey(key, mode);
    }
}

void InputManager::removeJoystickKeys(const std::vector<sf::Keyboard::Key> &keys, const Context &mode){
    for(const auto& key: keys) {
        removeJoystickKey(key, mode);
    }
}

void InputManager::removeAllKeys(const Context &mode) {
    switch(mode) {
    case CONTEXT_RUNNER:
        runnerKeyboardMap.clear();
        break;
    case CONTEXT_SHOOTER:
        shooterKeyboardMap.clear();
        break;
    }
}

void InputManager::removeAllJoystickKeys(const Context &mode){
    switch(mode) {
    case CONTEXT_RUNNER:
        runnerJoystickKeyboardMap.clear();
        break;
    case CONTEXT_SHOOTER:
        shooterJoystickKeyboardMap.clear();
        break;
    }
}

void InputManager::removeAllKeysUnbuf(const Context &mode) {
    switch(mode) {
    case CONTEXT_RUNNER:
        uRunnerKeyboardMap.clear();
        break;
    case CONTEXT_SHOOTER:
        uShooterKeyboardMap.clear();
        break;
    }
}

void InputManager::removeAllAxisUnbuf(const Context &mode) {
    switch(mode) {
    case CONTEXT_RUNNER:
        uRunnerJoystickAxisMap.clear();
        break;
    case CONTEXT_SHOOTER:
        uShooterJoystickAxisMap.clear();
        break;
    }
}

void InputManager::executeAction(const sf::Keyboard::Key &key, const Context &mode) {
    if(!hasKey(key, mode))
        return;

    switch(mode) {
    case CONTEXT_RUNNER:
        runnerKeyboardMap[key]();
        break;
    case CONTEXT_SHOOTER:
        shooterKeyboardMap[key]();
        break;
    }
}

void InputManager::executeJoystickKeyAction(const sf::Keyboard::Key &key, const Context &mode){
    if(!hasKey(key, mode))
        return;

    switch(mode) {
    case CONTEXT_RUNNER:
        runnerJoystickKeyboardMap[key]();
        break;
    case CONTEXT_SHOOTER:
        shooterJoystickKeyboardMap[key]();
        break;
    }
}

void InputManager::executeActionUnbuf(const Context &mode) {
    sf::Joystick::update();

    switch(mode) {
    case CONTEXT_RUNNER:
        // keyboard
        for(std::map<sf::Keyboard::Key, std::function<void(void)> >::iterator it = uRunnerKeyboardMap.begin(); it != uRunnerKeyboardMap.end(); ++it) {
            if(sf::Keyboard::isKeyPressed(it->first))
                it->second();
        }
        // joystick axis
        for(std::map<sf::Joystick::Axis, std::function<void(float)> >::iterator it = uRunnerJoystickAxisMap.begin(); it != uRunnerJoystickAxisMap.end(); ++it) {
            float axisPosition = sf::Joystick::getAxisPosition(JOYSTICK_NUMBER, it->first);
            if(axisPosition != 0) {
                it->second(axisPosition);
            }
        }
        break;
    case CONTEXT_SHOOTER:
        // keyboard
        for(std::map<sf::Keyboard::Key, std::function<void(void)> >::iterator it = uShooterKeyboardMap.begin(); it != uShooterKeyboardMap.end(); ++it) {
            if(sf::Keyboard::isKeyPressed(it->first))
                it->second();
        }
        // joystick axis
        for(std::map<sf::Joystick::Axis, std::function<void(float)> >::iterator it = uShooterJoystickAxisMap.begin(); it != uShooterJoystickAxisMap.end(); ++it) {
            float axisPosition = sf::Joystick::getAxisPosition(JOYSTICK_NUMBER, it->first);
            if(axisPosition != 0) {
                it->second(axisPosition);
            }
        }
        break;
    }
}



}
