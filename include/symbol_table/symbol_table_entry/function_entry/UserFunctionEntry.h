#include "include/symbol_table/symbol_table_entry/function_entry/FunctionEntry.h"

#ifndef USERFUNCTIONENTRY
#define USERFUNCTIONENTRY

/*
Provides a user function entry.
*/
class UserFunctionEntry final : public FunctionEntry {
    public:
        /*
        The Parameterized Constructor of this UserFunctionEntry. Sets this UserFunctionEntry function and its formal arguments.
        */
        UserFunctionEntry(Function function, std::list<FormalVariableEntry> formalArguments) : FunctionEntry(USER_FUNC, function, formalArguments){}; 
};

#endif