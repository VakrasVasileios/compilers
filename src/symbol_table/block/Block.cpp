#include <iostream>
#include "Block.h"

std::vector<SymbolTableEntry>
Block:: getSymbolTableEntries() const {
    return symbolTableEntries;
}

void
Block:: addSymbolTableEntry(SymbolTableEntry entry) {
    this->symbolTableEntries.push_back(entry);
}

// void    
// Block::SetIDVisibility(const std::string& _id, bool _visible) {
//     auto iter = ids.find(_id);
//     if (iter != ids.end()) {
//         (*iter).second.isVisible = _visible;
//     }
// }

// void
// Block::SetVisibilityAll(bool _visible) {
//     for (auto iter : ids)
//         iter.second.isVisible = _visible;
// }

// void
// Block::logBlock(void) {
//     for (auto iter : ids) {
//         std::cout << '\"' << iter.first << '\"' << " " << iter.second << std::endl;
//     }
// }