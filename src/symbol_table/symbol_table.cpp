#include "../../include/symbol_table/symbol_table.h"

std::vector<std::list<Block*> >
SymbolTable:: get_blocks_table() const {
    return blocks_table;
}

void
SymbolTable:: Insert(unsigned int scope, Block* block) {  
    blocks_table.push_back(std::list<Block*>());
    blocks_table[scope].push_back(block);
}

std::ostream&
operator<<(std::ostream& os, const SymbolTable symbol_table) {
    unsigned int scope = 0;
    for (auto blockList : symbol_table.blocks_table)
    {        
        os << "-----------     Scope #" << scope << "     -----------\n";
        for (auto block : blockList)
        {
            os << *block;
        }
        scope++;
    }

    return os;
}
