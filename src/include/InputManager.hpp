#ifndef _INPUTMANAGER_HPP_
#define _INPUTMANAGER_HPP_

#include <OISKeyboard.h>

#include <functional>
#include <vector>
#include <map>

namespace Cycleshooter {
class InputManager {
    std::map <OIS::KeyCode, std::function<void(void)> > keyMappings;

public:
    InputManager();
    virtual ~InputManager();

    void addOrUpdateBinding(const OIS::KeyCode& key, const std::function<void(void)>& action);
    void addOrUpdateBinding(const std::vector<OIS::KeyCode>& keys, const std::function<void(void)>& action);
    void removeBinding(const OIS::KeyCode& key);
    bool isBounded(const OIS::KeyCode& key) const;
    void executeAction(const OIS::KeyCode& key);
    void clearKeys();
};
}

#endif
