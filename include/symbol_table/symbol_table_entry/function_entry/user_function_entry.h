#ifndef USERFUNCTIONENTRY_H
#define USERFUNCTIONENTRY_H

#include "function_entry.h"

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
        The Parameterized Constructor of this UserFunctionEntry. Sets this UserFunctionEntry list of references of formal arguments.
        */
        UserFunctionEntry(std::string id, unsigned int line, unsigned int scope, std::list<FormalVariableEntry*> formalArguments)
            : FunctionEntry(USER_FUNC, id, line, scope, formalArguments){}; 
};

#endif