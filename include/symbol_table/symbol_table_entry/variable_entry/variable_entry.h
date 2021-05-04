#ifndef VARIABLEENTRY_H
#define VARIABLEENTRY_H

#include "../symbol_table_entry.h"

/*
Provides a scope type for this VariableEntry.
*/
enum ScopeType {
    LOCAL_T,
    GLOBAL_T,
    FORMAL_T
};

/*
Provides a symbol table entry, abstract variable class extension.
*/
class VariableEntry : public SymbolTableEntry {   
    public:
        /*
        The Parameterized Constructor of this VariableEntry.
        */
        VariableEntry(ExprType expr_type, ScopeType scope_type, std::string id, unsigned int line, unsigned int scope)
        : SymbolTableEntry(expr_type, id, line, scope), type(scope_type) {};  

        virtual ~VariableEntry() = default;
        /*
        Returns the scope type of this VariableEntry.
        */
        ScopeType       get_scope_type() const;
        /*
        Logs this VariableEntry.
        */
        void            Log(std::ostream& output);
    protected:
        /*
        The scope type of this VariableEntry.
        */
        ScopeType type;
        /*
        Returns a string representation of this VariableEntry scope type.
        */
        std::string ScopeTypeToString() const;
};

#endif