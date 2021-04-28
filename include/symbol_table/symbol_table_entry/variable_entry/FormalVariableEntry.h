#ifndef FORMALVARIABLEENTRY_H
#define FORMALVARIABLEENTRY_H

#include "../SymbolTableEntry.h"
#include "../../../id/Variable.h"

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