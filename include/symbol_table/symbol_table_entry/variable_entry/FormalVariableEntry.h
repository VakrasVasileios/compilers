#include "include/symbol_table/symbol_table_entry/SymbolTableEntry.h"
#include "include/id/Variable.h"

#ifndef FORMALVARIABLEENTRY
#define FORMALVARIABLEENTRY

/*
Provides a formal variable entry.
*/
class FormalVariableEntry final : public SymbolTableEntry {
    public:
        /*
        The Parameterized Constructor of this FormalVariableEntry. Sets this FormalVariableEntry variable.
        */
        FormalVariableEntry(Variable variable) : SymbolTableEntry(FORMAL_VAR, variable) {};
};

#endif