#ifndef _LOGGING_HPP_
#define _LOGGING_HPP_

#include <cstdio>
#include <stdexcept>

namespace Cycleshooter {

/**
  * Log debug messages to stdout.
  */

#ifdef LOGGING_ENABLED

#define LOG(...)\
    do {\
        printf("INFO: ");\
        printf(__VA_ARGS__);\
        printf("\n");\
    } while(false)

#else

#define LOG(...)

#endif

[[ noreturn ]]
inline void LOG_FATAL(const char* reason, ...) {
    char buf[1024];
    va_list va;
    printf("FATAL: ");
    va_start(va, reason);
    vsprintf(buf, reason, va);
    va_end(va);
    puts("");

    throw std::runtime_error(buf);
}

}

#endif
