#ifndef GLOBALVARIABLEENTRY_H
#define GLOBALVARIABLEENTRY_H

#include "variable_entry.h"

/*
Provides a global variable entry.
*/
class GlobalVariableEntry final : public VariableEntry {
    public:
        /*
        The Parameterized Constructor of this GlobalVariableEntry.
        */
        GlobalVariableEntry(std::string id, unsigned int line, unsigned int scope) : VariableEntry(VAR, GLOBAL_T, id, line, scope) {};
};

#endif