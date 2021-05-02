#ifndef FUNCTIONENTRY_H
#define FUNCTIONENTRY_H

#include "../symbol_table_entry.h"
#include "../variable_entry/formal_variable_entry.h"
#include <list>

/*
Provides an abstract function symbol table entry.
*/
class FunctionEntry : public SymbolTableEntry {
    public:
        /*
        The Parameterized FunctionEntry Constructor.
        */
        FunctionEntry(ExprType type, std::string id, unsigned int line, unsigned int scope) : 
        SymbolTableEntry(type, id, line, scope) {};
        /*
        The Parameterized FunctionEntry Constructor. Sets this FunctionEntry list of references of formal arguments.
        */
        FunctionEntry(ExprType type, std::string id, unsigned int line, unsigned int scope, std::list<FormalVariableEntry*> formalArguments) : 
        SymbolTableEntry(type, id, line, scope), formal_arguments(formalArguments) {};

        virtual                             ~FunctionEntry() = default;
        /*
        Returns a read access to this FunctionEntry list of references formal arguments.
        */
        std::list<FormalVariableEntry*>      get_formal_arguments() const; 
    private:
        const std::list<FormalVariableEntry*> formal_arguments;
};

#endif
