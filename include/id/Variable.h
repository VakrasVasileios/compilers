#include "include/id/Id.h"

#ifndef VARIABLE
#define VARIABLE

/*
Provides a variable id.
*/
class Variable final : public Id {
    public:
        Variable(std::string name, unsigned int line, unsigned int scope) : Id(name, line, scope) {};
};

#endif




