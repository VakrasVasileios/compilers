#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#include <iostream>

#ifdef DELOG
    #define dlog(message) std::cout << message << std::endl
#else
    #define dlog(message)
#endif

#endif