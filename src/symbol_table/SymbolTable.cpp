#include "../../include/symbol_table/SymbolTable.h"

std::vector<std::list<Block> >
SymbolTable:: getBlocksTable() const {
    return blocksTable;
}

void
SymbolTable:: insert(unsigned int scope, Block block) {
    this->blocksTable[scope].push_back(block);
}

std::ostream&
operator<<(std::ostream& os, const SymbolTable symbolTable) {
    unsigned int scope = 0;
    for (auto blockList : symbolTable.getBlocksTable())
    {
       os << "\n-----------    Scope #" << scope << "    -----------\n";
       for (auto block : blockList)
       {
           os << block;
       }
       scope++;
    }

    return os;
}
