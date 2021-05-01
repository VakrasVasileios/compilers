#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
#include "../block/Block.h"

/*
Provides a symbol table for keeping lists of blocks.
*/
class SymbolTable final {
    private:
        std::vector<std::list<Block*> > blocksTable;
    public:
        /*
        The Default Constructor of this SymbolTable.
        */
        SymbolTable() = default;
        /*
        The Default Destructor of this SymbolTable.
        */
        ~SymbolTable() = default;
        /*
        Inserts a block on a scope to this SymbolTable.
        */
        void                            insert(unsigned int scope, Block* block);

        friend std::ostream&            operator<<(std::ostream& os, const SymbolTable symbolTable);
};

#endif