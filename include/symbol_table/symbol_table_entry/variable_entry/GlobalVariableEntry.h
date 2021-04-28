#ifndef GLOBALVARIABLEENTRY_H
#define GLOBALVARIABLEENTRY_H

#include "../SymbolTableEntry.h"
#include "../../../id/Variable.h"

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