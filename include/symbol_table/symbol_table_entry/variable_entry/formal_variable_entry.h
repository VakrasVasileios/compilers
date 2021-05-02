#ifndef FORMALVARIABLEENTRY_H
#define FORMALVARIABLEENTRY_H

#include "variable_entry.h"

/*
Provides a formal variable entry.
*/
class FormalVariableEntry final : public VariableEntry {
    public:
        /*
        The Parameterized Constructor of this FormalVariableEntry.
        */
        FormalVariableEntry(std::string id, unsigned int line, unsigned int scope) : VariableEntry(VAR, FORMAL_T, id, line, scope) {};
};

#endif