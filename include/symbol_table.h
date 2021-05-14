#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
#include "block.h"

/*
Provides a table of block lists across many scopes.
*/
class SymbolTable final {
public:
    SymbolTable() = default;
    ~SymbolTable() = default;
    /*
    Returns this SymbolTable blocks table.
    */
    std::vector<std::list<Block*> > get_blocks_table() const;
    /*
    Inserts block at a scope on this SymbolTable.
    */
    void                            Insert(unsigned int scope, Block* block);

    friend std::ostream&            operator<<(std::ostream& os, const SymbolTable symbol_table);
private:
    std::vector<std::list<Block*> > blocks_table;

    static bool                     ContainsAnEntry(std::list<Block*> blockList);
    static bool                     ContainsOnlyHiddenVariables(std::list<Block*> blockList);
};

#endif