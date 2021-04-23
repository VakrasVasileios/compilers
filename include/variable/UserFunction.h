#include "Variable.h"

#ifndef USERFUNCTION
#define USERFUNCTION

/*
Provides a user function.
*/
class UserFunction : public Variable {
    public:
        /*
        The Parameterized Constructor of this UserFunction. Sets this UserFunction name, line, scope.
        */
        UserFunction(std::string name, unsigned int line, unsigned int scope) : Variable(USER_FUNC, name, line, scope) {};
};

#endif