#ifndef _INPUTMANAGER_HPP_
#define _INPUTMANAGER_HPP_

#include <functional>
#include <map>
#include <vector>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Context.hpp"

// TODO: find a better way to handle this
#define JOYSTICK_NUMBER 0

namespace Cycleshooter {
class InputManager {
    // constructor and copy functions
    InputManager(){}
    InputManager(const InputManager&) = delete;
    void operator=(const InputManager&) = delete;

    // buffered keyboard
    std::map<sf::Keyboard::Key, std::function<void(void)> > runnerKeyboardMap;
    std::map<sf::Keyboard::Key, std::function<void(void)> > shooterKeyboardMap;

    // unbuffered keyboard
    std::map<sf::Keyboard::Key, std::function<void(void)> > uRunnerKeyboardMap;
    std::map<sf::Keyboard::Key, std::function<void(void)> > uShooterKeyboardMap;

    // unbuffered joystick axis
    std::map<sf::Joystick::Axis, std::function<void(float)> > uRunnerJoystickAxisMap;
    std::map<sf::Joystick::Axis, std::function<void(float)> > uShooterJoystickAxisMap;

    bool hasKey(const sf::Keyboard::Key& key, const Context& mode);

    bool hasKeyUnbuf(const sf::Keyboard::Key& key, const Context& mode);

    bool hasAxisUnbuf(const sf::Joystick::Axis& axis, const Context& mode);

public:
    static InputManager& instance();

    void addKey(const sf::Keyboard::Key& key, const std::function<void(void)> &action);

    void addKeyUnbuf(const sf::Keyboard::Key& key, const std::function<void(void)> &action);

    void addAxisUnbuf(const sf::Joystick::Axis& axis, const std::function<void(float)> &action);

    void addKey(const sf::Keyboard::Key& key, const Context& mode, const std::function<void(void)> &action);

    void addKeyUnbuf(const sf::Keyboard::Key& key, const Context& mode, const std::function<void(void)> &action);

    void addAxisUnbuf(const sf::Joystick::Axis& axis, const Context& mode, const std::function<void(float)> &action);

    void addKeys(const std::vector<sf::Keyboard::Key>& keys, const std::function<void(void)> &action);

    void addKeysUnbuf(const std::vector<sf::Keyboard::Key>& keys, const std::function<void(void)> &action);

    void addKeys(const std::vector<sf::Keyboard::Key>& keys, const Context& mode, const std::function<void(void)> &action);

    void addKeysUnbuf(const std::vector<sf::Keyboard::Key>& keys, const Context& mode, const std::function<void(void)> &action);

    void removeKey(const sf::Keyboard::Key& key, const Context& mode);

    void removeKeyUnbuf(const sf::Keyboard::Key& key, const Context& mode);

    void removeAxisUnbuf(const sf::Joystick::Axis& axis, const Context& mode);

    void removeKeys(const std::vector<sf::Keyboard::Key>& keys, const Context& mode);

    void removeAllKeys(const Context& mode);

    void removeAllKeysUnbuf(const Context& mode);

    void removeAllAxisUnbuf(const Context& mode);

    void executeAction(const sf::Keyboard::Key& key, const Context& mode);

    void executeActionUnbuf(const Context& mode);

};
}

#endif
