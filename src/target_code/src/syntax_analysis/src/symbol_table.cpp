#include "symbol_table.h"

namespace target_code
{
    namespace syntax_analysis 
    {
    std::vector<std::list<Block*> >
    SymbolTable:: get_blocks_table() const {
        return blocks_table;
    }

    void
    SymbolTable:: Insert(unsigned int scope, Block* block) {  
        PRECONDITION(scope >= 0);
        PRECONDITION(block != nullptr);
        blocks_table.push_back(std::list<Block*>());
        blocks_table[scope].push_back(block);
    }

    bool
    SymbolTable:: ContainsAnEntry(std::list<Block*> blockList) {
        bool contains = false;
        for (auto block : blockList) {
            if (!block->get_symbols().empty())
                contains = true;
        }
        return contains;
    }

    std::ostream&
    operator<<(std::ostream& os, const SymbolTable symbol_table) {
        unsigned int scope = 0;

        for (auto blockList : symbol_table.blocks_table)
        {        
            if (symbol_table.ContainsAnEntry(blockList))
            {
                os 
                << "-----------     Scope #" << scope << "     -----------\n";
                for (auto block : blockList)
                {
                    os << *block;
                }    
            }
            scope++;
        }

        return os;
    }
    }
}