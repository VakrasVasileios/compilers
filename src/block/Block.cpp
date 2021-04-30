#include <iostream>
#include <list>
#include "../../include/block/Block.h"

std::list<SymbolTableEntry>&
Block:: getSymbolTableEntries() {
    return symbolTableEntries;
}

void
Block:: addSymbolTableEntry(SymbolTableEntry entry) {
    this->symbolTableEntries.push_back(entry);
}

void
Block:: hide() {
    for (auto i : symbolTableEntries) {
        i.setActive(false);
    }
}

std::ostream&
operator<<(std::ostream& os, Block block) {
    for (auto entry : block.getSymbolTableEntries())
    {
        os << entry << "\n";
    }
    
    return os;
}
