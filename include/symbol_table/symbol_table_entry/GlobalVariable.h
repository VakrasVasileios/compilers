#include "include/symbol_table/symbol_table_entry/SymbolTableEntry.h"
#include "include/id/Variable.h"

#ifndef GLOBALVARIABLE
#define GLOBALVARIABLE

/*
Provides a global variable.
*/
class GlobalVariable final : public SymbolTableEntry {
    public:
        /*
        The Parameterized Constructor of this GlobalVariable. Sets this GlobalVariable variable.
        */
        GlobalVariable(Variable variable) : SymbolTableEntry(GLOBAL_VAR, variable, true) {};
};

#endif