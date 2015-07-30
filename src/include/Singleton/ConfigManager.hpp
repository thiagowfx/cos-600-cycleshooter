#ifndef _CONFIGMANAGER_HPP_
#define _CONFIGMANAGER_HPP_

#include <fstream>
#include <string>

#include "toml.h"
#include "Logging.hpp"
#include "Singleton.hpp"

namespace Cycleshooter {

class ConfigManager {
    SINGLETON(ConfigManager)

    toml::Value v;

public:
    std::string getStr(const std::string& key) const;

    int getInt(const std::string& key) const;

    double getDouble(const std::string& key) const;

    bool getBool(const std::string& key) const;

    template<class T>
    T get(const std::string& key) const {
        try {
            return v.get<T>(key);
        } catch (...) {
            LOG_FATAL("No such key exists: %s", key.c_str());
        }
    }
};

}

#endif
