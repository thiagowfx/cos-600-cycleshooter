#ifndef _INPUTMANAGER_HPP_
#define _INPUTMANAGER_HPP_

#include <functional>
#include <map>
#include <vector>

#include <OgreLogManager.h>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Context.hpp"

namespace Cycleshooter {
/**
 * @brief The InputManager class A singleton class to handle all the user input (mouse, keyboard and joystick).
 * Each different input type of events is stored in a different map for efficiency and flexibility purposes.
 * A rich set of methods is also provided so the client application can be more easily set up.
 */
class InputManager {
    // constructor and copy functions to make this class a singleton
    InputManager(){}
    InputManager(const InputManager&) = delete;
    void operator=(const InputManager&) = delete;

public:
    static InputManager& instance() {
        static InputManager instance;
        return instance;
    }

private:
    /**
     * @brief JOYSTICK_NUMBER The joystick (number) associated with the maps of this class.
     */
    unsigned int JOYSTICK_NUMBER = 0;

    // buffered keyboard keys
    std::map<sf::Keyboard::Key, std::function<void(void)> > keyboardMap[2];

    // buffered joystick buttons
    std::map<unsigned int, std::function<void(void)> > joystickButtonMap[2];

    // unbuffered keyboard keys
    std::map<sf::Keyboard::Key, std::function<void(void)> > uKeyboardMap[2];

    // unbuffered joystick axis
    std::map<sf::Joystick::Axis, std::function<void(float)> > uJoystickAxisMap[2];

    bool hasKey(const sf::Keyboard::Key& key, const Context& mode);

    bool hasJoystickButton(unsigned int button, const Context& mode);

    bool hasKeyUnbuf(const sf::Keyboard::Key& key, const Context& mode);

    bool hasJoystickAxisUnbuf(const sf::Joystick::Axis& axis, const Context& mode);

public:

    void addKey(const sf::Keyboard::Key& key, const std::function<void(void)> &action);

    void addJoystickButton(unsigned int button, const std::function<void(void)> &action);

    void addKeyUnbuf(const sf::Keyboard::Key& key, const std::function<void(void)> &action);

    void addJoystickAxisUnbuf(const sf::Joystick::Axis& axis, const std::function<void(float)> &action);

    void addJoystickButton(unsigned int button, const Context& mode, const std::function<void(void)> &action);

    void addKey(const sf::Keyboard::Key& key, const Context& mode, const std::function<void(void)> &action);

    void addKeyUnbuf(const sf::Keyboard::Key& key, const Context& mode, const std::function<void(void)> &action);

    void addJoystickAxisUnbuf(const sf::Joystick::Axis& axis, const Context& mode, const std::function<void(float)> &action);

    void addKeys(const std::vector<sf::Keyboard::Key>& keys, const std::function<void(void)> &action);

    void addJoystickButtons(const std::vector<unsigned int>& buttons, const std::function<void(void)> &action);

    void addKeysUnbuf(const std::vector<sf::Keyboard::Key>& keys, const std::function<void(void)> &action);

    void addKeys(const std::vector<sf::Keyboard::Key>& keys, const Context& mode, const std::function<void(void)> &action);

    void addJoystickButtons(const std::vector<unsigned int>& buttons, const Context& mode, const std::function<void(void)> &action);

    void addKeysUnbuf(const std::vector<sf::Keyboard::Key>& keys, const Context& mode, const std::function<void(void)> &action);

    void removeKey(const sf::Keyboard::Key& key, const Context& mode);

    void removeJoystickButton(unsigned int button, const Context& mode);

    void removeKeyUnbuf(const sf::Keyboard::Key& key, const Context& mode);

    void removeJoystickAxisUnbuf(const sf::Joystick::Axis& axis, const Context& mode);

    void removeAllKeys();

    void removeAllKeys(const Context& mode);

    void removeAllJoystickButtons();

    void removeAllJoystickButtons(const Context& mode);

    void removeAllKeysUnbuf();

    void removeAllKeysUnbuf(const Context& mode);

    void removeAllJoystickAxisUnbuf();

    void removeAllJoystickAxisUnbuf(const Context& mode);

    void executeKeyAction(const sf::Keyboard::Key& key, const Context& mode);

    void executeJoystickButtonAction(unsigned int button, const Context& mode);

    void executeActionsUnbuf(const Context& mode);

    void setJoystickNumber(unsigned int number);

    void reset();

    /**
     * @brief detectJoystick Tries to detect the correct joystick.
     * It must have both X and Y axis, and at least 1 button.
     */
    void detectJoystick();
};
}

#endif
