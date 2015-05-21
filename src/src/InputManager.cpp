#include "InputManager.hpp"

namespace Cycleshooter {

InputManager::InputManager() {
}

void InputManager::addOrUpdateBinding(const OIS::KeyCode &key, const std::function<void(void)> &action) {
    keyBindings[key] = action;
}

void InputManager::addOrUpdateBinding(const std::vector<OIS::KeyCode> &keys, const std::function<void ()> &action) {
    for(const auto& key: keys) {
        addOrUpdateBinding(key, action);
    }
}

void InputManager::removeBinding(const OIS::KeyCode &key) {
    if(hasBinding(key)) {
        keyBindings.erase(key);
    }
}

bool InputManager::hasBinding(const OIS::KeyCode &key) const {
    return keyBindings.find(key) != keyBindings.end();
}

void InputManager::executeAction(const OIS::KeyCode &key) {
    if(hasBinding(key)) {
        keyBindings[key]();
    }
}

void InputManager::removeBindings() {
    keyBindings.clear();
}

}
