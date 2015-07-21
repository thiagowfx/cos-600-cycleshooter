#ifndef _SINGLETON_HPP_
#define _SINGLETON_HPP_

/**
 * Make a class a singleton.
 * Usage:
 *
 *      class Name {
 *          SINGLETON(Name)
 *      };
 *
 *  or
 *
 *      class Name {
 *          SINGLETON_NC(Name)
 *      };
 */

namespace Cycleshooter {

#define _SINGLETON_COMMON(C)\
public:\
    static C& instance() {\
        static C instance;\
        return instance;\
    }\
private:\
    C(const C&) = delete;\
    C& operator=(const C&) = delete;

#define SINGLETON(C)\
    _SINGLETON_COMMON(C)\
    C();

// No constructor.
#define SINGLETON_NC(C)\
    _SINGLETON_COMMON(C)\
    C(){}

}

#endif
