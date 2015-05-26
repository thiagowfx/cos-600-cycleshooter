#ifndef _INPUTMANAGER_HPP_
#define _INPUTMANAGER_HPP_

#include <functional>
#include <map>
#include <vector>
#include <SFML/Window.hpp>

#include "Context.hpp"

namespace Cycleshooter {
class InputManager {
    // constructor and copy functions
    InputManager(){}
    InputManager(const InputManager&) = delete;
    void operator=(const InputManager&) = delete;

    // buffered
    std::map<sf::Keyboard::Key, std::function<void(void)> > runnerKeyboardMap;
    std::map<sf::Keyboard::Key, std::function<void(void)> > shooterKeyboardMap;

    // unbuffered
    std::map<sf::Keyboard::Key, std::function<void(void)> > uRunnerKeyboardMap;
    std::map<sf::Keyboard::Key, std::function<void(void)> > uShooterKeyboardMap;

    bool hasKey(const sf::Keyboard::Key& key, const Context& mode);

    bool hasKeyUnbuf(const sf::Keyboard::Key& key, const Context& mode);

public:
    static InputManager& instance();

    void addKey(const sf::Keyboard::Key& key, const std::function<void(void)> &action);

    void addKeyUnbuf(const sf::Keyboard::Key& key, const std::function<void(void)> &action);

    void addKey(const sf::Keyboard::Key& key, const Context& mode, const std::function<void(void)> &action);

    void addKeyUnbuf(const sf::Keyboard::Key& key, const Context& mode, const std::function<void(void)> &action);

    void addKeys(const std::vector<sf::Keyboard::Key>& keys, const std::function<void(void)> &action);

    void addKeysUnbuf(const std::vector<sf::Keyboard::Key>& keys, const std::function<void(void)> &action);

    void addKeys(const std::vector<sf::Keyboard::Key>& keys, const Context& mode, const std::function<void(void)> &action);

    void addKeysUnbuf(const std::vector<sf::Keyboard::Key>& keys, const Context& mode, const std::function<void(void)> &action);

    void removeKey(const sf::Keyboard::Key& key, const Context& mode);

    void removeKeyUnbuf(const sf::Keyboard::Key& key, const Context& mode);

    void removeKeys(const std::vector<sf::Keyboard::Key>& keys, const Context& mode);

    void removeAllKeys(const Context& mode);

    void removeAllKeysUnbuf(const Context& mode);

    void executeAction(const sf::Keyboard::Key& key, const Context& mode);

    void executeActionUnbuf(const Context& mode);

};
}

#endif
