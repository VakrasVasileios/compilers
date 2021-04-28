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
        UserFunctionEntry(Function function) : FunctionEntry(USER_FUNC, function){};
        /*
        The Parameterized Constructor of this UserFunctionEntry. Sets this UserFunctionEntry function and its formal arguments.
        */
        UserFunctionEntry(Function function, std::list<FormalVariableEntry> formalArguments) : FunctionEntry(USER_FUNC, function, formalArguments){}; 
};

#endif