#include "InputManager.hpp"

namespace Cycleshooter {

bool InputManager::hasKey(const sf::Keyboard::Key &key, const Context &mode) const {
    return keyboardMap[mode].find(key) != keyboardMap[mode].end();
}

bool InputManager::hasJoystickButton(int button, const Context &mode) const {
    return joystickButtonMap[mode].find(button) != joystickButtonMap[mode].end();
}

bool InputManager::hasKeyUnbuf(const sf::Keyboard::Key &key, const Context &mode) const {
    return uKeyboardMap[mode].find(key) != uKeyboardMap[mode].end();
}

bool InputManager::hasKeyRotationUnbuf(const sf::Keyboard::Key &key, const Context &mode) const {
    return urKeyboardMap[mode].find(key) != urKeyboardMap[mode].end();
}

bool InputManager::hasJoystickAxisUnbuf(const sf::Joystick::Axis& axis, const Context &mode) const {
    return uJoystickAxisMap[mode].find(axis) != uJoystickAxisMap[mode].end();
}

void InputManager::addKey(const sf::Keyboard::Key &key, const std::function<void ()> &action) {
    addKey(key, CONTEXT_RUNNER, action);
    addKey(key, CONTEXT_SHOOTER, action);
}

void InputManager::addJoystickButton(int button, const std::function<void ()> &action){
    addJoystickButton(button, CONTEXT_RUNNER, action);
    addJoystickButton(button, CONTEXT_SHOOTER, action);
}

void InputManager::addKeyUnbuf(const sf::Keyboard::Key &key, const std::function<void ()> &action) {
    addKeyUnbuf(key, CONTEXT_RUNNER, action);
    addKeyUnbuf(key, CONTEXT_SHOOTER, action);
}

void InputManager::addKeyRotationUnbuf(const sf::Keyboard::Key &key, const std::function<void ()> &action) {
    addKeyRotationUnbuf(key, CONTEXT_RUNNER, action);
    addKeyRotationUnbuf(key, CONTEXT_SHOOTER, action);
}

void InputManager::addJoystickAxisUnbuf(const sf::Joystick::Axis &axis, const std::function<void (float)> &action) {
    addJoystickAxisUnbuf(axis, CONTEXT_RUNNER, action);
    addJoystickAxisUnbuf(axis, CONTEXT_SHOOTER, action);
}

void InputManager::addJoystickButton(int button, const Context &mode, const std::function<void ()> &action){
    joystickButtonMap[mode][button] = action;
}

void InputManager::addKey(const sf::Keyboard::Key &key, const Context &mode, const std::function<void ()> &action) {
    keyboardMap[mode][key] = action;
}

void InputManager::addKeyUnbuf(const sf::Keyboard::Key &key, const Context &mode, const std::function<void ()> &action) {
    uKeyboardMap[mode][key] = action;
}

void InputManager::addKeyRotationUnbuf(const sf::Keyboard::Key &key, const Context &mode, const std::function<void ()> &action) {
    urKeyboardMap[mode][key] = action;
}

void InputManager::addJoystickAxisUnbuf(const sf::Joystick::Axis &axis, const Context &mode, const std::function<void (float)> &action) {
    uJoystickAxisMap[mode][axis] = action;
}

void InputManager::addKeys(const std::vector<sf::Keyboard::Key> &keys, const std::function<void ()> &action) {
    for(const auto& key: keys) {
        addKey(key, action);
    }
}

void InputManager::addJoystickButtons(const std::vector<int> &buttons, const std::function<void ()> &action) {
    for(const auto& button: buttons) {
        addJoystickButton(button, action);
    }
}

void InputManager::addJoystickAxisUnbuf(const std::vector<sf::Joystick::Axis> &axises, const std::function<void(float)> &action) {
    for(const auto& axis: axises) {
        addJoystickAxisUnbuf(axis, action);
    }
}

void InputManager::addKeysUnbuf(const std::vector<sf::Keyboard::Key> &keys, const std::function<void ()> &action) {
    for(const auto& key: keys) {
        addKeyUnbuf(key, action);
    }
}

void InputManager::addKeysRotationUnbuf(const std::vector<sf::Keyboard::Key> &keys, const std::function<void ()> &action) {
    for(const auto& key: keys) {
        addKeyRotationUnbuf(key, action);
    }
}

void InputManager::addKeys(const std::vector<sf::Keyboard::Key> &keys, const Context &mode, const std::function<void ()> &action) {
    for(const auto& key: keys) {
        addKey(key, mode, action);
    }
}

void InputManager::addJoystickButtons(const std::vector<int> &buttons, const Context &mode, const std::function<void ()> &action) {
    for(const auto& button: buttons) {
        addJoystickButton(button, mode, action);
    }
}

void InputManager::addJoystickAxisUnbuf(const std::vector<sf::Joystick::Axis> &axises, const Context &mode, const std::function<void(float)> &action) {
    for(const auto& axis: axises) {
        addJoystickAxisUnbuf(axis, mode, action);
    }
}

void InputManager::addKeysUnbuf(const std::vector<sf::Keyboard::Key> &keys, const Context &mode, const std::function<void ()> &action) {
    for(const auto& key: keys) {
        addKeyUnbuf(key, mode, action);
    }
}

void InputManager::addKeysRotationUnbuf(const std::vector<sf::Keyboard::Key> &keys, const Context &mode, const std::function<void ()> &action) {
    for(const auto& key: keys) {
        addKeyRotationUnbuf(key, mode, action);
    }
}

void InputManager::removeKey(const sf::Keyboard::Key &key, const Context &mode) {
    if(hasKey(key, mode)) {
        keyboardMap[mode].erase(key);
    }
}

void InputManager::removeJoystickButton(int button, const Context &mode) {
    if(hasJoystickButton(button, mode)) {
        joystickButtonMap[mode].erase(button);
    }
}

void InputManager::removeKeyUnbuf(const sf::Keyboard::Key &key, const Context &mode) {
    if(hasKeyUnbuf(key, mode)) {
        uKeyboardMap[mode].erase(key);
    }
}

void InputManager::removeKeyRotationUnbuf(const sf::Keyboard::Key &key, const Context &mode) {
    if(hasKeyRotationUnbuf(key, mode)) {
        urKeyboardMap[mode].erase(key);
    }
}

void InputManager::removeJoystickAxisUnbuf(const sf::Joystick::Axis &axis, const Context &mode) {
    if(hasJoystickAxisUnbuf(axis, mode)) {
        uJoystickAxisMap[mode].erase(axis);
    }
}

void InputManager::removeAllKeys() {
    removeAllKeys(CONTEXT_RUNNER);
    removeAllKeys(CONTEXT_SHOOTER);
}

void InputManager::removeAllKeys(const Context &mode) {
    keyboardMap[mode].clear();
}

void InputManager::removeAllJoystickButtons() {
    removeAllJoystickButtons(CONTEXT_RUNNER);
    removeAllJoystickButtons(CONTEXT_SHOOTER);
}

void InputManager::removeAllJoystickButtons(const Context &mode) {
    joystickButtonMap[mode].clear();
}

void InputManager::removeAllKeysUnbuf() {
    removeAllKeysUnbuf(CONTEXT_RUNNER);
    removeAllKeysUnbuf(CONTEXT_SHOOTER);
}

void InputManager::removeAllKeysRotationUnbuf() {
    removeAllKeysRotationUnbuf(CONTEXT_RUNNER);
    removeAllKeysRotationUnbuf(CONTEXT_SHOOTER);
}

void InputManager::removeAllKeysUnbuf(const Context &mode) {
    uKeyboardMap[mode].clear();
}

void InputManager::removeAllKeysRotationUnbuf(const Context &mode) {
    urKeyboardMap[mode].clear();
}

void InputManager::removeAllJoystickAxisUnbuf() {
    removeAllJoystickAxisUnbuf(CONTEXT_RUNNER);
    removeAllJoystickAxisUnbuf(CONTEXT_SHOOTER);
}

void InputManager::removeAllJoystickAxisUnbuf(const Context &mode) {
    uJoystickAxisMap[mode].clear();
}

void InputManager::executeKeyAction(const sf::Keyboard::Key &key, const Context &mode) {
    if(hasKey(key, mode)) {
        keyboardMap[mode][key]();
    }
}

void InputManager::executeJoystickButtonAction(int button, const Context &mode) {
    if(hasJoystickButton(button, mode)) {
        joystickButtonMap[mode][button]();
    }
}

void InputManager::executeActionsUnbuf(const Context &mode) {
    sf::Joystick::update();

    // keyboard
    for(std::map<sf::Keyboard::Key, std::function<void(void)> >::iterator it = uKeyboardMap[mode].begin(); it != uKeyboardMap[mode].end(); ++it) {
        if(sf::Keyboard::isKeyPressed(it->first)) {
            it->second();
        }
    }

    // joystick axis
    for(std::map<sf::Joystick::Axis, std::function<void(float)> >::iterator it = uJoystickAxisMap[mode].begin(); it != uJoystickAxisMap[mode].end(); ++it) {
        float axisPosition = sf::Joystick::getAxisPosition(JOYSTICK_NUMBER, it->first);
        if(axisPosition != 0.0) {
            it->second(axisPosition);
        }
    }
}

void InputManager::executeActionsRotationUnbuf(const Context &mode) {
    //rotation keyboard
    for(std::map<sf::Keyboard::Key, std::function<void(void)> >::iterator it = urKeyboardMap[mode].begin(); it != urKeyboardMap[mode].end(); ++it) {
        if(sf::Keyboard::isKeyPressed(it->first)) {
            it->second();
        }
    }
}

void InputManager::setJoystickNumber(int number) {
    JOYSTICK_NUMBER = number;
}

bool InputManager::isMovementKeyPressed() const {
    static const std::vector<sf::Keyboard::Key> movementKeys = {
        sf::Keyboard::W,
        sf::Keyboard::A,
        sf::Keyboard::S,
        sf::Keyboard::D,
        sf::Keyboard::Left,
        sf::Keyboard::Right,
        sf::Keyboard::Up,
        sf::Keyboard::Down
    };

    return isKeyPressed(movementKeys);
}

bool InputManager::isJoystickMovementAxisPressed() const {
    sf::Joystick::update();

    static const std::vector<sf::Joystick::Axis> movementAxis = {
        sf::Joystick::X,
        sf::Joystick::Y,
        sf::Joystick::U,
        sf::Joystick::V,
        sf::Joystick::PovX,
        sf::Joystick::PovY
    };

    for(const auto& axis: movementAxis) {
        if(sf::Joystick::getAxisPosition(JOYSTICK_NUMBER, axis) != 0.0) {
            return true;
        }
    }

    return false;
}

bool InputManager::isKeyPressed(const std::vector<sf::Keyboard::Key> &keys) const {
    for(sf::Keyboard::Key key: keys) {
        if(sf::Keyboard::isKeyPressed(key))
            return true;
    }
    return false;
}

void InputManager::reset() {
    removeAllKeys();
    removeAllKeysUnbuf();
    removeAllKeysRotationUnbuf();
    removeAllJoystickButtons();
    removeAllJoystickAxisUnbuf();
}

void InputManager::updateJoystickNumber() {
    if(ConfigManager::instance().getBool("InputManager.try_detect_joystick")) {
        LOG("Trying to detect a Joystick");
        bool joystickDetected = false;

        for(int n = 0; n < sf::Joystick::Count; ++n) {
            if(sf::Joystick::isConnected(n) &&
                    sf::Joystick::hasAxis(n, sf::Joystick::X) &&
                    sf::Joystick::hasAxis(n, sf::Joystick::Y) &&
                    sf::Joystick::getButtonCount(n) >= 2) {
                LOG("Detected a Joystick. Setting its number to %d", n);
                setJoystickNumber(n);
                joystickDetected = true;
                break;
            }
        }

        if(!joystickDetected) {
            LOG("No compatible joystick could be detected");
        }
    }
    else {
        JOYSTICK_NUMBER = ConfigManager::instance().getInt("InputManager.joystick_number");
        LOG("Setting the joystick number to %d. It has been obtained from the config file", JOYSTICK_NUMBER);

        if(sf::Joystick::isConnected(JOYSTICK_NUMBER)) {
            LOG("The configured joystick IS connected");
        }
        else {
            LOG("The configured joystick is NOT available.");
        }
    }
}

}
