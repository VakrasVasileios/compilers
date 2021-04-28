#ifndef LOCALVARIABLEENTRY_H
#define LOCALVARIABLEENTRY_H

#include "../SymbolTableEntry.h"
#include "../../../id/Variable.h"

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