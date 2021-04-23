#include "include/symbol_table/symbol_table_entry/SymbolTableEntry.h"
#include "include/id/Variable.h"

#ifndef FORMALVARIABLE
#define FORMALVARIABLE

/*
Provides a formal variable.
*/
class FormalVariable final : public SymbolTableEntry {
    public:
        /*
        The Parameterized Constructor of this FormalVariable. Sets this FormalVariable variable.
        */
        FormalVariable(Variable variable) : SymbolTableEntry(FORMAL_VAR, variable, true) {};
};

#endif