#ifndef FUNCTIONENTRY_H
#define FUNCTIONENTRY_H

#include "../SymbolTableEntry.h"
#include "../variable_entry/FormalVariableEntry.h"
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
        FunctionEntry(SymbolType type, std::string id, unsigned int line, unsigned int scope) : SymbolTableEntry(type, id, line, scope) {};
        /*
        The Parameterized FunctionEntry Constructor. Sets this FunctionEntry formal arguments.
        */
        FunctionEntry(SymbolType type, std::string id, unsigned int line, unsigned int scope, std::list<FormalVariableEntry> formalArguments) : 
        SymbolTableEntry(type, id, line, scope), formalArguments(formalArguments) {};
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
