#ifndef LOCALVARIABLEENTRY_H
#define LOCALVARIABLEENTRY_H

#include "../symbol_table_entry.h"

/*
Provides a local variable entry.
*/
class LocalVariableEntry final : public SymbolTableEntry {
    public:
        /*
        The Parameterized Constructor of this LocalVariableEntry.
        */
        LocalVariableEntry(std::string id, unsigned int line, unsigned int scope) : SymbolTableEntry(LOCAL_VAR, id, line, scope) {};
};

#endif