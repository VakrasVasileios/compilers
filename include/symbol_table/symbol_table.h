#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
#include "../block/block.h"

class SymbolTable final {
public:
    SymbolTable() = default;
    ~SymbolTable() = default;

    std::vector<std::list<Block*> > get_blocks_table() const;

    void                            Insert(unsigned int scope, Block* block);

    friend std::ostream&            operator<<(std::ostream& os, const SymbolTable symbol_table);
private:
    std::vector<std::list<Block*> > blocks_table;
};

#endif