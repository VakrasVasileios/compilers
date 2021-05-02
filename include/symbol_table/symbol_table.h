#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
#include "../block/block.h"

/*
Provides a symbol table, for keeping a table of lists of blocks references.
*/
class SymbolTable final {
    public:
        SymbolTable() = default;
        ~SymbolTable() = default;
        /*
        Returns a read access to this SymbolTable table,
        of lists of blocks references.
        */
        std::vector<std::list<Block*> > get_blocks_table() const;
        /*
        Inserts a block reference at a block list on a scope to this SymbolTable.
        */
        void                            Insert(unsigned int scope, Block* block);

        friend std::ostream&            operator<<(std::ostream& os, const SymbolTable symbol_table);
    private:
        std::vector<std::list<Block*> > blocks_table;
};

#endif