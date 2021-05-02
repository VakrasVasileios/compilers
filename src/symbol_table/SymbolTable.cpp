#include "../../include/symbol_table/SymbolTable.h"

std::vector<std::list<Block*> >
SymbolTable:: getBlocksTable() const {
    return blocksTable;
}

void
SymbolTable:: insert(unsigned int scope, Block* block) {  
    // if (scope > blocksTable.size()-1)
    //     blocksTable.push_back(std::list<Block*>());
    blocksTable.push_back(std::list<Block*>());
    blocksTable[scope].push_back(block);
}

std::ostream&
operator<<(std::ostream& os, const SymbolTable symbolTable) {
    unsigned int scope = 0;
    for (auto blockList : symbolTable.blocksTable)
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
