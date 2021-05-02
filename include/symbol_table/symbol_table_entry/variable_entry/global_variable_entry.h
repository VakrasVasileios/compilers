#ifndef GLOBALVARIABLEENTRY_H
#define GLOBALVARIABLEENTRY_H

#include "../symbol_table_entry.h"

/*
Provides a global variable entry.
*/
class GlobalVariableEntry final : public SymbolTableEntry {
    public:
        /*
        The Parameterized Constructor of this GlobalVariableEntry.
        */
        GlobalVariableEntry(std::string id, unsigned int line, unsigned int scope) : SymbolTableEntry(GLOBAL_VAR, id, line, scope) {};
};

#endif