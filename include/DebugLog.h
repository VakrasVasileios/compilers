#include <iostream>

#ifndef DEBUG_LOG
#define DEBUG_LOG

#ifdef DELOG
    #define dlog(message) std::cout << message << std::endl
#else
    #define dlog(message)
#endif

#endif