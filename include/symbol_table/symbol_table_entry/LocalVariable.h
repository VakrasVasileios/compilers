#include "include/symbol_table/symbol_table_entry/SymbolTableEntry.h"
#include "include/id/Variable.h"

#ifndef LOCALVARIABLE
#define LOCALVARIABLE

/*
Provides a local variable.
*/
class LocalVariable final : public SymbolTableEntry {
    public:
        /*
        The Parameterized Constructor of this LocalVariable. Sets this LocalVariable function.
        */
        LocalVariable(Variable variable) : SymbolTableEntry(LOCAL_VAR, variable, true) {};
};

#endif