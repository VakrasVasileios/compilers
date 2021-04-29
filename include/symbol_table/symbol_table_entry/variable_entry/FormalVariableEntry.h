#ifndef FORMALVARIABLEENTRY_H
#define FORMALVARIABLEENTRY_H

#include "../SymbolTableEntry.h"

/*
Provides a formal variable entry.
*/
class FormalVariableEntry final : public SymbolTableEntry {
    public:
        /*
        The Parameterized Constructor of this FormalVariableEntry.
        */
        FormalVariableEntry(std::string id, unsigned int line, unsigned int scope) : SymbolTableEntry(FORMAL_VAR, id, line, scope) {};
};

#endif