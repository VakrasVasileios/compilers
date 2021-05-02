#ifndef LOCALVARIABLEENTRY_H
#define LOCALVARIABLEENTRY_H

#include "variable_entry.h"

/*
Provides a local variable entry.
*/
class LocalVariableEntry final : public VariableEntry {
    public:
        /*
        The Parameterized Constructor of this LocalVariableEntry.
        */
        LocalVariableEntry(std::string id, unsigned int line, unsigned int scope) : VariableEntry(VAR, LOCAL_T, id, line, scope) {};
};

#endif