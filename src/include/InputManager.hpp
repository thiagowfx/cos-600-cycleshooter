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
/**
 * A singleton class to handle all the user input (mouse, keyboard and joystick).
 * Each different input type of events is stored in a different map for efficiency and flexibility purposes.
 * A rich set of methods is also provided so the client application can be more easily set up.
 */
class InputManager {
    // constructor and copy functions
    InputManager(){}
    InputManager(const InputManager&) = delete;
    void operator=(const InputManager&) = delete;

    // buffered keyboard
    std::map<sf::Keyboard::Key, std::function<void(void)> > runnerKeyboardMap;
    std::map<sf::Keyboard::Key, std::function<void(void)> > shooterKeyboardMap;

    // buffered joystick keys
    std::map<unsigned int, std::function<void(void)> > runnerJoystickKeyboardMap;
    std::map<unsigned int, std::function<void(void)> > shooterJoystickKeyboardMap;

    // unbuffered keyboard
    std::map<sf::Keyboard::Key, std::function<void(void)> > uRunnerKeyboardMap;
    std::map<sf::Keyboard::Key, std::function<void(void)> > uShooterKeyboardMap;

    // unbuffered joystick axis
    std::map<sf::Joystick::Axis, std::function<void(float)> > uRunnerJoystickAxisMap;
    std::map<sf::Joystick::Axis, std::function<void(float)> > uShooterJoystickAxisMap;

    bool hasKey(const sf::Keyboard::Key& key, const Context& mode);

    bool hasJoystickKey(unsigned int button, const Context& mode);

    bool hasKeyUnbuf(const sf::Keyboard::Key& key, const Context& mode);

    bool hasAxisUnbuf(const sf::Joystick::Axis& axis, const Context& mode);

public:
    static InputManager& instance();

    void addKey(const sf::Keyboard::Key& key, const std::function<void(void)> &action);

    void addJoystickKey(unsigned int button, const std::function<void(void)> &action);

    void addKeyUnbuf(const sf::Keyboard::Key& key, const std::function<void(void)> &action);

    void addAxisUnbuf(const sf::Joystick::Axis& axis, const std::function<void(float)> &action);

    void addJoystickKey(unsigned int button, const Context& mode, const std::function<void(void)> &action);

    void addKey(const sf::Keyboard::Key& key, const Context& mode, const std::function<void(void)> &action);

    void addKeyUnbuf(const sf::Keyboard::Key& key, const Context& mode, const std::function<void(void)> &action);

    void addAxisUnbuf(const sf::Joystick::Axis& axis, const Context& mode, const std::function<void(float)> &action);

    void addKeys(const std::vector<sf::Keyboard::Key>& keys, const std::function<void(void)> &action);

    void addJoystickKeys(const std::vector<unsigned int>& buttons, const std::function<void(void)> &action);

    void addKeysUnbuf(const std::vector<sf::Keyboard::Key>& keys, const std::function<void(void)> &action);

    void addKeys(const std::vector<sf::Keyboard::Key>& keys, const Context& mode, const std::function<void(void)> &action);

    void addJoystickKeys(const std::vector<unsigned int>& buttons, const Context& mode, const std::function<void(void)> &action);

    void addKeysUnbuf(const std::vector<sf::Keyboard::Key>& keys, const Context& mode, const std::function<void(void)> &action);

    void removeKey(const sf::Keyboard::Key& key, const Context& mode);

    void removeJoystickKey(unsigned int button, const Context& mode);

    void removeKeyUnbuf(const sf::Keyboard::Key& key, const Context& mode);

    void removeAxisUnbuf(const sf::Joystick::Axis& axis, const Context& mode);

    void removeKeys(const std::vector<sf::Keyboard::Key>& keys, const Context& mode);

    void removeJoystickKeys(const std::vector<unsigned int>& buttons, const Context& mode);

    void removeAllKeys(const Context& mode);

    void removeAllJoystickKeys(const Context& mode);

    void removeAllKeysUnbuf(const Context& mode);

    void removeAllAxisUnbuf(const Context& mode);

    void executeAction(const sf::Keyboard::Key& key, const Context& mode);

    void executeJoystickKeyAction(unsigned int button, const Context& mode);

    void executeActionUnbuf(const Context& mode);

};
}

#endif
