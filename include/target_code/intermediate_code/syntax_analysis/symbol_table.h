#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
#include "../../../../util/contract/contract.h"
#include "block.h"

namespace syntax_analysis {
    /**
     * @brief A table of block lists across many scopes.
    **/ 
    class SymbolTable final {
    public:
        SymbolTable() = default;
        ~SymbolTable() = default;
        /*
        Returns this SymbolTable blocks table.
        */
        /**
         * @brief Returns a read access to this SymbolTable blocks table,
         * not null
         * 
         * @return a read access to this SymbolTable blocks table,
         * not null
        **/ 
        std::vector<std::list<Block*> > get_blocks_table() const;
        /**
         * @brief Inserts a block at a scope on this SymbolTable.
         * 
         * @param scope the scope on which to insert a block, greater
         * or equal to zero.
         * @param block the block to be inserted, not null 
        **/ 
        void                            Insert(unsigned int scope, Block* block);

        friend std::ostream&            operator<<(std::ostream& os, const SymbolTable symbol_table);
    private:
        std::vector<std::list<Block*> > blocks_table;

        static bool                     ContainsAnEntry(std::list<Block*> blockList);
    };
}

#endif