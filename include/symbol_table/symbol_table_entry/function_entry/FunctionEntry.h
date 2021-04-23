#include "include/symbol_table/symbol_table_entry/SymbolTableEntry.h"
#include "include/symbol_table/symbol_table_entry/variable_entry/FormalVariableEntry.h"
#include "include/id/Function.h"
#include <list>

#ifndef FUNCTIONENTRY
#define FUNCTIONENTRY

/*
Provides an abstract function symbol table entry.
*/
class FunctionEntry : public SymbolTableEntry {
    private:
        const std::list<FormalVariableEntry> formalArguments;
    public:
        /*
        The Parameterized FunctionEntry Constructor. Sets this FunctionEntry formal arguments.
        */
        FunctionEntry(SymbolType type, Function function, std::list<FormalVariableEntry> formalArguments) : 
        SymbolTableEntry(type, function), formalArguments(formalArguments) {};
        /*
        The Default FunctionEntry Destructor.
        */
        virtual                     ~FunctionEntry() = default;
        /*
        Returns this FunctionEntry formal arguments.
        */
        std::list<FormalVariableEntry>   getFormalArguments() const; 
};

#endif