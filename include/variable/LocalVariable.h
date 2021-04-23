#include "Variable.h"

#ifndef LOCALVARIABLE
#define LOCALVARIABLE

/*
Provides a local variable.
*/
class LocalVariable : public Variable {
    public:
        /*
        The Parameterized Constructor of this LocalVariable. Sets this LocalVariable name, line, scope.
        */
        LocalVariable(std::string name, unsigned int line, unsigned int scope) : Variable(LOCAL_VAR, name, line, scope) {};
};

#endif