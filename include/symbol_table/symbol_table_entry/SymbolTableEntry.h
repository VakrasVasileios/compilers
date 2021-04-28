#ifndef SYMBOLTABLEENTRY_H
#define SYMBOLTABLEENTRY_H

#include "../../id/Id.h"
#include <string.h>

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

/* Provides a symbol table entry.
*/
class SymbolTableEntry {
    private:
        const SymbolType    type;
        const Id            id;
        bool                active;

        std::string         typeToString(SymbolType type) const;
    public:
        /*
        The Parameterized Constructor of this SymbolTableEntry. Sets this SymbolTableEntry type, id and activeness.
        */
        SymbolTableEntry(SymbolType type, Id id) : type(type), id(id), active(true) {};
        /*
        The Default Destructor of this SymbolTableEntry.
        */
        virtual                 ~SymbolTableEntry() = default;
        /*
        Returns this SymbolTableEntry type.
        */
        SymbolType              getType() const;
        /*
        Returns this SymbolTableEntry id.
        */
        Id                      getId() const;
        /*
        Returns this SymbolTableEntry activeness.
        */
        bool                    isActive() const;
        /*
        Sets this SymbolTableEntry activeness.
        */
        void                    setActive(bool active);
        
        friend std::ostream&    operator<<(std::ostream& os, const SymbolTableEntry symbolTableEntry);
};

#endif