#ifndef LOCALVARIABLEENTRY_H
#define LOCALVARIABLEENTRY_H

#include "include/symbol_table/symbol_table_entry/SymbolTableEntry.h"
#include "include/id/Variable.h"

/*
Provides a local variable entry.
*/
class LocalVariableEntry final : public SymbolTableEntry {
    public:
        /*
        The Parameterized Constructor of this LocalVariableEntry. Sets this LocalVariableEntry function.
        */
        LocalVariableEntry(Variable variable) : SymbolTableEntry(LOCAL_VAR, variable) {};
};

#endif