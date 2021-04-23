#include "include/symbol_table/symbol_table_entry/SymbolTableEntry.h"
#include "include/id/Function.h"

#ifndef USERFUNCTION
#define USERFUNCTION

/*
Provides a user function.
*/
class UserFunction final : public SymbolTableEntry {
    public:
        /*
        The Parameterized Constructor of this UserFunction. Sets this UserFunction function.
        */
        UserFunction(Function function) : SymbolTableEntry(USER_FUNC, function, true) {};
};

#endif