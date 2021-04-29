#include <iostream>
#include <list>
#include "../../include/block/Block.h"

std::list<SymbolTableEntry>
Block:: getSymbolTableEntries() const {
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



// void    
// Block::setSymbolTableEntryVisibility(const std::string name, bool visible) {
//     auto iter = entries.find(name);
//     if (iter != entries.end()) {
//         (*iter).second.setActive(visible);
//     }
// }

// void
// Block::setVisibilityAll(bool visible) {
//     for (auto iter : entries)
//         iter.second.setActive(visible);
// }

// void
// Block::logBlock(void) {
//     for (auto iter : entries) {
//         std::cout << iter.second << std::endl;
//     }
// }
