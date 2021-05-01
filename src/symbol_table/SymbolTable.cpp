#include "../../include/symbol_table/SymbolTable.h"

void
SymbolTable:: insert(unsigned int scope, Block* block) {  
    blocksTable.push_back(std::list<Block*>());
    blocksTable[scope].push_back(block);
}

std::ostream&
operator<<(std::ostream& os, const SymbolTable symbolTable) {
    unsigned int scope = 0;
    for (auto blockList : symbolTable.blocksTable)
    {
       os << "\n-----------    Scope #" << scope << "    -----------\n";
       for (auto block : blockList)
       {
           os << *block;
       }
       scope++;
    }

    return os;
}
