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

void InputManager::addKeys(const std::vector<sf::Keyboard::Key> &keys, const std::function<void ()> &action) {
    for(const auto& key: keys) {
        addKey(key, action);
    }
}

void InputManager::addKeys(const std::vector<sf::Keyboard::Key> &keys, const Context &mode, const std::function<void ()> &action) {
    for(const auto& key: keys) {
        addKey(key, mode, action);
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

void InputManager::removeKeys(const std::vector<sf::Keyboard::Key> &keys, const Context &mode) {
    for(const auto& key: keys) {
        removeKey(key, mode);
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



}
