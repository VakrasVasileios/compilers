#include "Variable.h"

#ifndef GLOBALVARIABLE
#define GLOBALVARIABLE

/*
Provides a global variable.
*/
class GlobalVariable : public Variable {
    public:
        /*
        The Parameterized Constructor of this GlobalVariable. Sets this GlobalVariable name, line, scope.
        */
        GlobalVariable(std::string name, unsigned int line, unsigned int scope) : Variable(GLOBAL_VAR, name, line, scope) {};
};

#endif