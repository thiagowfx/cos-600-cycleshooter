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

bool InputManager::hasJoystickButton(unsigned int button, const Context &mode){
    switch(mode) {
    case CONTEXT_RUNNER:
        return runnerJoystickButtonMap.find(button) != runnerJoystickButtonMap.end();
        break;
    case CONTEXT_SHOOTER:
        return shooterJoystickButtonMap.find(button) != shooterJoystickButtonMap.end();
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

bool InputManager::hasJoystickAxisUnbuf(const sf::Joystick::Axis& axis, const Context &mode) {
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

void InputManager::addJoystickButton(unsigned int button, const std::function<void ()> &action){
    addJoystickButton(button, CONTEXT_RUNNER, action);
    addJoystickButton(button, CONTEXT_SHOOTER, action);
}

void InputManager::addKeyUnbuf(const sf::Keyboard::Key &key, const std::function<void ()> &action) {
    addKeyUnbuf(key, CONTEXT_RUNNER, action);
    addKeyUnbuf(key, CONTEXT_SHOOTER, action);
}

void InputManager::addJoystickAxisUnbuf(const sf::Joystick::Axis &axis, const std::function<void (float)> &action) {
    addJoystickAxisUnbuf(axis, CONTEXT_RUNNER, action);
    addJoystickAxisUnbuf(axis, CONTEXT_SHOOTER, action);
}

void InputManager::addJoystickButton(unsigned int button, const Context &mode, const std::function<void ()> &action){
    switch(mode) {
    case CONTEXT_RUNNER:
        runnerJoystickButtonMap[button] = action;
        break;
    case CONTEXT_SHOOTER:
        shooterJoystickButtonMap[button] = action;
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

void InputManager::addJoystickAxisUnbuf(const sf::Joystick::Axis &axis, const Context &mode, const std::function<void (float)> &action) {
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

void InputManager::addJoystickButtons(const std::vector<unsigned int> &buttons, const std::function<void ()> &action){
    for(const auto& button: buttons) {
        addJoystickButton(button, action);
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

void InputManager::addJoystickButtons(const std::vector<unsigned int> &buttons, const Context &mode, const std::function<void ()> &action){
    for(const auto& button: buttons) {
        addJoystickButton(button, mode, action);
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

void InputManager::removeJoystickButton(unsigned int button, const Context &mode){
    if(!hasJoystickButton(button, mode))
        return;

    switch(mode) {
    case CONTEXT_RUNNER:
        runnerJoystickButtonMap.erase(button);
        break;
    case CONTEXT_SHOOTER:
        shooterJoystickButtonMap.erase(button);
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

void InputManager::removeJoystickAxisUnbuf(const sf::Joystick::Axis &axis, const Context &mode) {
    if(!hasJoystickAxisUnbuf(axis, mode))
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

void InputManager::removeAllJoystickButtons(const Context &mode){
    switch(mode) {
    case CONTEXT_RUNNER:
        runnerJoystickButtonMap.clear();
        break;
    case CONTEXT_SHOOTER:
        shooterJoystickButtonMap.clear();
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

void InputManager::removeAllJoystickAxisUnbuf(const Context &mode) {
    switch(mode) {
    case CONTEXT_RUNNER:
        uRunnerJoystickAxisMap.clear();
        break;
    case CONTEXT_SHOOTER:
        uShooterJoystickAxisMap.clear();
        break;
    }
}

void InputManager::executeKeyAction(const sf::Keyboard::Key &key, const Context &mode) {
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

void InputManager::executeJoystickButtonAction(unsigned int button, const Context &mode){
    if(!hasJoystickButton(button, mode))
        return;

    switch(mode) {
    case CONTEXT_RUNNER:
        runnerJoystickButtonMap[button]();
        break;
    case CONTEXT_SHOOTER:
        shooterJoystickButtonMap[button]();
        break;
    }
}

void InputManager::executeActionsUnbuf(const Context &mode) {
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

void InputManager::setJoystickNumber(unsigned int number) {
    JOYSTICK_NUMBER = number;
}

void InputManager::detectJoystick() {
    std::cout << "--> Controller: Detecting Joystick <--" << std::endl;

    for(unsigned n = 0; n < sf::Joystick::Count; ++n) {
        if(sf::Joystick::isConnected(n) &&
           sf::Joystick::hasAxis(n, sf::Joystick::X) &&
           sf::Joystick::hasAxis(n, sf::Joystick::Y) &&
           sf::Joystick::getButtonCount(n) >= 1) {
            std::cout << " |-> Setting joystick to number " + std::to_string(n) << std::endl;
            setJoystickNumber(n);
            break;
        }
    }
}

}
