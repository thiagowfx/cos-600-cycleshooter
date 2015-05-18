#ifndef _INPUTMANAGER_HPP_
#define _INPUTMANAGER_HPP_

#include <OIS.h>
#include <functional>
#include <map>
#include <vector>

namespace Cycleshooter {
class InputManager {
    std::map <OIS::KeyCode, std::function<void(void)> > keyBindings;

public:
    InputManager();

    void addOrUpdateBinding(const OIS::KeyCode& key, const std::function<void(void)>& action);
    void addOrUpdateBinding(const std::vector<OIS::KeyCode>& keys, const std::function<void(void)>& action);
    void clearBindings();
    void executeAction(const OIS::KeyCode& key);
    bool hasBinding(const OIS::KeyCode& key) const;
    void removeBinding(const OIS::KeyCode& key);
};
}

#endif
