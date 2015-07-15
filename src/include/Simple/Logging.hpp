#ifndef _LOGGING_HPP_
#define _LOGGING_HPP_

#include <cstdio>

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

}

#endif
