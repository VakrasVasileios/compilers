#include "include/symbol_table/symbol_table_entry/SymbolTableEntry.h"
#include "include/id/Variable.h"

#ifndef GLOBALVARIABLEENTRY
#define GLOBALVARIABLEENTRY

/*
Provides a global variable entry.
*/
class GlobalVariableEntry final : public SymbolTableEntry {
    public:
        /*
        The Parameterized Constructor of this GlobalVariableEntry. Sets this GlobalVariableEntry variable.
        */
        GlobalVariableEntry(Variable variable) : SymbolTableEntry(GLOBAL_VAR, variable) {};
};

#endif