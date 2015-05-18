#include "InputManager.hpp"

namespace Cycleshooter {

InputManager::InputManager() {
}

InputManager::~InputManager() {
}

void InputManager::addOrUpdateBinding(const OIS::KeyCode &key, const std::function<void(void)> &action) {
    keyMappings[key] = action;
}

void InputManager::addOrUpdateBinding(const std::vector<OIS::KeyCode> &keys, const std::function<void ()> &action) {
    for(const auto& key: keys) {
        addOrUpdateBinding(key, action);
    }
}

void InputManager::removeBinding(const OIS::KeyCode &key) {
    if(isBounded(key)) {
        keyMappings.erase(key);
    }
}

bool InputManager::isBounded(const OIS::KeyCode &key) const {
    return keyMappings.find(key) != keyMappings.end();
}

void InputManager::executeAction(const OIS::KeyCode &key) {
    if(isBounded(key)) {
        keyMappings[key]();
    }
}

void InputManager::clearKeys() {
    keyMappings.clear();
}

}
