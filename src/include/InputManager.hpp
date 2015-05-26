#ifndef _INPUTMANAGER_HPP_
#define _INPUTMANAGER_HPP_

#include "Context.hpp"
#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <SFML/Window.hpp>

namespace Cycleshooter {
class InputManager {
    // constructor and copy functions
    InputManager(){}
    InputManager(const InputManager&) = delete;
    void operator=(const InputManager&) = delete;

    std::map<sf::Keyboard::Key, std::function<void(void)> > runnerKeyboardMap;
    std::map<sf::Keyboard::Key, std::function<void(void)> > shooterKeyboardMap;

    bool hasKey(const sf::Keyboard::Key& key, const Context& mode);

public:
    static InputManager& instance();

    void addKey(const sf::Keyboard::Key& key, const std::function<void(void)> &action);

    void addKey(const sf::Keyboard::Key& key, const Context& mode, const std::function<void(void)> &action);

    void addKeys(const std::vector<sf::Keyboard::Key>& keys, const std::function<void(void)> &action);

    void addKeys(const std::vector<sf::Keyboard::Key>& keys, const Context& mode, const std::function<void(void)> &action);

    void removeKey(const sf::Keyboard::Key& key, const Context& mode);

    void removeKeys(const std::vector<sf::Keyboard::Key>& keys, const Context& mode);

    void removeAllKeys(const Context& mode);

    void executeAction(const sf::Keyboard::Key& key, const Context& mode);

};
}

#endif
