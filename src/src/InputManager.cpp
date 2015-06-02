#include "InputManager.hpp"

namespace Cycleshooter {

bool InputManager::hasKey(const sf::Keyboard::Key &key, const Context &mode) {
    return keyboardMap[mode].find(key) != keyboardMap[mode].end();
}

bool InputManager::hasJoystickButton(unsigned int button, const Context &mode){
    return joystickButtonMap[mode].find(button) != joystickButtonMap[mode].end();
}

bool InputManager::hasKeyUnbuf(const sf::Keyboard::Key &key, const Context &mode) {
    return uKeyboardMap[mode].find(key) != uKeyboardMap[mode].end();
}

bool InputManager::hasJoystickAxisUnbuf(const sf::Joystick::Axis& axis, const Context &mode) {
    return uJoystickAxisMap[mode].find(axis) != uJoystickAxisMap[mode].end();
}

InputManager &InputManager::instance() {
    /*
     * Guaranteed to be destroyed
     * Instantiated on first use
     */
    static InputManager instance;
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
    joystickButtonMap[mode][button] = action;
}

void InputManager::addKey(const sf::Keyboard::Key &key, const Context &mode, const std::function<void ()> &action) {
    keyboardMap[mode][key] = action;
}

void InputManager::addKeyUnbuf(const sf::Keyboard::Key &key, const Context &mode, const std::function<void ()> &action) {
    uKeyboardMap[mode][key] = action;
}

void InputManager::addJoystickAxisUnbuf(const sf::Joystick::Axis &axis, const Context &mode, const std::function<void (float)> &action) {
    uJoystickAxisMap[mode][axis] = action;
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

    keyboardMap[mode].erase(key);
}

void InputManager::removeJoystickButton(unsigned int button, const Context &mode){
    if(!hasJoystickButton(button, mode))
        return;

    joystickButtonMap[mode].erase(button);
}

void InputManager::removeKeyUnbuf(const sf::Keyboard::Key &key, const Context &mode) {
    if(!hasKeyUnbuf(key, mode))
        return;

    uKeyboardMap[mode].erase(key);
}

void InputManager::removeJoystickAxisUnbuf(const sf::Joystick::Axis &axis, const Context &mode) {
    if(!hasJoystickAxisUnbuf(axis, mode))
        return;

    uJoystickAxisMap[mode].erase(axis);
}

void InputManager::removeAllKeys(const Context &mode) {
    keyboardMap[mode].clear();
}

void InputManager::removeAllJoystickButtons(const Context &mode){
    joystickButtonMap[mode].clear();
}

void InputManager::removeAllKeysUnbuf(const Context &mode) {
    uKeyboardMap[mode].clear();
}

void InputManager::removeAllJoystickAxisUnbuf(const Context &mode) {
    uJoystickAxisMap[mode].clear();
}

void InputManager::executeKeyAction(const sf::Keyboard::Key &key, const Context &mode) {
    if(!hasKey(key, mode))
        return;

    keyboardMap[mode][key]();
}

void InputManager::executeJoystickButtonAction(unsigned int button, const Context &mode){
    if(!hasJoystickButton(button, mode))
        return;

    joystickButtonMap[mode][button]();
}

void InputManager::executeActionsUnbuf(const Context &mode) {
    sf::Joystick::update();

    // keyboard
    for(std::map<sf::Keyboard::Key, std::function<void(void)> >::iterator it = uKeyboardMap[mode].begin(); it != uKeyboardMap[mode].end(); ++it) {
        if(sf::Keyboard::isKeyPressed(it->first))
            it->second();
    }

    // joystick axis
    for(std::map<sf::Joystick::Axis, std::function<void(float)> >::iterator it = uJoystickAxisMap[mode].begin(); it != uJoystickAxisMap[mode].end(); ++it) {
        float axisPosition = sf::Joystick::getAxisPosition(JOYSTICK_NUMBER, it->first);
        if(axisPosition != 0) {
            it->second(axisPosition);
        }
    }
}

void InputManager::setJoystickNumber(unsigned int number) {
    JOYSTICK_NUMBER = number;
}

void InputManager::detectJoystick() {
    std::cout << "--> InputManager: Detecting Joystick <--" << std::endl;

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
