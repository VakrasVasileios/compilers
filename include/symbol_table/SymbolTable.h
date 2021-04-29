#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
#include "block/Block.h"

// /*
// Provides a symbol table for keeping lists of blocks.
// */
class SymbolTable final {
    private:
        std::vector<std::list<Block> > blocksTable;
    public:
        /*
        The Default Constructor of this SymbolTable.
        */
        SymbolTable() = default;
        /*
        Inserts a block, at the end of a block list, on a scope to this SymbolTable.
        */
        void                pushBlock(unsigned int scope, Block block);
        /*
        Inserts an entry on a scope to this SymbolTable.
        */
        void                insert(unsigned int scope, SymbolTableEntry entry);
        /*
        Searches for an entry on this SymbolTable. Returns entry if it's found, else null
        */
        SymbolTableEntry    lookup(SymbolTableEntry entry);
        /*
        Hides all the entries on a scope on this SymbolTable.
        */
        void                hide(unsigned int scope);
};

// class SymbolTable : public std::vector<BlockList> {
// public:
//     SymbolTable() = default;
//     ~SymbolTable() = default;

//     void        logSymbolTable(void);
// };

#endif