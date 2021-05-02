#ifndef SYMBOLTABLEENTRY_H
#define SYMBOLTABLEENTRY_H

#include <string>
#include <iostream>

/* 
Provides a type for a symbol table entry.
*/
enum SymbolType {
    /*
    The type of a Library Function.
    */
    LIB_FUNC,
    /*
    The type of a User Function.
    */
    USER_FUNC,
    /*
    The type of a Local Variable.
    */
    LOCAL_VAR,
    /*
    The type of a Global Variable.
    */
    GLOBAL_VAR,
    /*
    The type of a Formal Argument.
    */
    FORMAL_VAR
};

/* 
Provides a symbol table entry with an id, found at a line, on a scope, with a type.
*/
class SymbolTableEntry {
    public:
        /*
        The Parameterized Constructor of this SymbolTableEntry. Sets this SymbolTableEntry type, id, line, scope and activeness.
        */
        SymbolTableEntry(SymbolType type, std::string id, unsigned int line, unsigned int scope) :
            type(type), id(id), line(line), scope(scope), active(true) {};
        virtual                 ~SymbolTableEntry() = default;
        /*
        Returns a read access to this SymbolTableEntry type.
        */
        SymbolType              get_type() const;
        /* 
        Returns a read access to this SymbolTableEntry id.
        */
        std::string             get_id() const;
        /* 
        Returns a read access to this SymbolTableEntry line.
        */
        unsigned int            get_line() const;
        /* 
        Returns a read access to this SymbolTableEntry scope.
        */
        unsigned int            get_scope() const;
        /*
        Returns a read access to this SymbolTableEntry activeness.
        */
        bool                    is_active() const;
        /*
        Sets this SymbolTableEntry activeness.
        */
        void                    set_active(bool active);
        
        friend std::ostream&    operator<<(std::ostream& os, const SymbolTableEntry symbol_table_entry);
    private:
        const SymbolType    type;
        const std::string   id;
        const unsigned int  line;
        const unsigned int  scope;
        bool                active;

        std::string         TypeToString(SymbolType type) const;
};

#endif