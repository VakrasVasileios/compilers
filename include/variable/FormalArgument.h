#include "Variable.h"

#ifndef FORMALARGUMENT
#define FORMALARGUMENT

/*
Provides a formal argument.
*/
class FormalArgument : public Variable {
    public:
        /*
        The Parameterized Constructor of this FormalArgument. Sets this FormalArgument name, line, scope.
        */
        FormalArgument(std::string name, unsigned int line, unsigned int scope) : Variable(FORMAL_ARG, name, line, scope) {};
};

#endif