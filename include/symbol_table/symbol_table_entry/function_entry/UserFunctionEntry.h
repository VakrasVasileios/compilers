#ifndef USERFUNCTIONENTRY_H
#define USERFUNCTIONENTRY_H

#include "FunctionEntry.h"

/*
Provides a user function entry.
*/
class UserFunctionEntry final : public FunctionEntry {
    public:
        /*
        The Parameterized Constructor of this UserFunctionEntry.
        */
        UserFunctionEntry(std::string id, unsigned int line, unsigned int scope) : FunctionEntry(USER_FUNC, id, line, scope){};
        /*
        The Parameterized Constructor of this UserFunctionEntry. Sets this UserFunctionEntry function and its formal arguments.
        */
        UserFunctionEntry(std::string id, unsigned int line, unsigned int scope, std::list<FormalVariableEntry> formalArguments)
            : FunctionEntry(USER_FUNC, id, line, scope, formalArguments){}; 
};

#endif