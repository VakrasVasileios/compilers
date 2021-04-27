#include "include/id/Id.h"

#ifndef FUNCTION
#define FUNCTION

/*
Provides a function id.
*/
class Function final : public Id {
    public:
        Function(std::string name, unsigned int line, unsigned int scope) : Id(name, line, scope) {};
};

#endif