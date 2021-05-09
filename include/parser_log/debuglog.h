#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#include <iostream>

#ifdef DELOG
    #define DLOG(message) std::cout << message << std::endl
#else
    #define DLOG(message)
#endif

#endif