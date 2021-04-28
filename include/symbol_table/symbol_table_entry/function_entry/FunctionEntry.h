#ifndef FUNCTIONENTRY_H
#define FUNCTIONENTRY_H

#include "../SymbolTableEntry.h"
#include "../variable_entry/FormalVariableEntry.h"
#include "../../../id/Function.h"
#include <list>

/*
Provides an abstract function symbol table entry.
*/
class FunctionEntry : public SymbolTableEntry {
    private:
        const std::list<FormalVariableEntry> formalArguments;
    public:
        /*
        The Parameterized FunctionEntry Constructor.
        */
        FunctionEntry(SymbolType type, Function function) : SymbolTableEntry(type, function) {};
        /*
        The Parameterized FunctionEntry Constructor. Sets this FunctionEntry formal arguments.
        */
        FunctionEntry(SymbolType type, Function function, std::list<FormalVariableEntry> formalArguments) : 
        SymbolTableEntry(type, function), formalArguments(formalArguments) {};
        /*
        The Default FunctionEntry Destructor.
        */
        virtual                             ~FunctionEntry() = default;
        /*
        Returns this FunctionEntry formal arguments.
        */
        std::list<FormalVariableEntry>      getFormalArguments() const; 
};

#endif