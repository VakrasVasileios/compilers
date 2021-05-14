#include "../include/symbol_table.h"

std::vector<std::list<Block*> >
SymbolTable:: get_blocks_table() const {
    return blocks_table;
}

void
SymbolTable:: Insert(unsigned int scope, Block* block) {  
    blocks_table.push_back(std::list<Block*>());
    blocks_table[scope].push_back(block);
}

bool
SymbolTable:: ContainsAnEntry(std::list<Block*> blockList) {
    bool contains = false;
    for (auto block : blockList) {
        if (!block->get_entries().empty())
            contains = true;
    }
    return contains;
}

bool
SymbolTable:: ContainsOnlyHiddenVariables(std::list<Block*> blockList) {
    unsigned int blocks_with_only_hidden_vars_count = 0;
    for (auto block : blockList) {
        if (block->ContainsOnlyHiddenVars())
            blocks_with_only_hidden_vars_count++;    
    }
    
    return blocks_with_only_hidden_vars_count == blockList.size();
}

std::ostream&
operator<<(std::ostream& os, const SymbolTable symbol_table) {
    unsigned int scope = 0;
    for (auto blockList : symbol_table.blocks_table)
    {        
        if (symbol_table.ContainsAnEntry(blockList) /* && !symbol_table.ContainsOnlyHiddenVariables(blockList) */)
        {
            os << "-----------     Scope #" << scope << "     -----------\n";
            for (auto block : blockList)
            {
                os << *block;
            }    
        }
        scope++;
    }

    return os;
}
