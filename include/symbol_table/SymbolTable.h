#include <vector>
#include <map>
#include "include/symbol_table/block/Block.h"

#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

/*
Provides a symbol table for mapping symbol table entries with their scope.
*/
class SymbolTable final {
    private:
        std::map<int, std::vector<Block> > blocksMap;
    public:
        /*
        The Default Constructor of this SymbolTable.
        */
        SymbolTable() = default;
        /*
        Inserts an entry on a scope to this SymbolTable.
        */
        void                Insert(int scope, SymbolTableEntry entry);
        /*
        Searches for an entry on this SymbolTable. Returns entry if it's found, else null
        */
        SymbolTableEntry    LookUp(SymbolTableEntry entry);
        /*
        Hides all the entries on a scope on this SymbolTable.
        */
        void                Hide(int scope);
};

// class SymbolTable : public std::vector<BlockList> {
// public:
//     SymbolTable() = default;
//     ~SymbolTable() = default;

//     void        logSymbolTable(void);
// };

#endif