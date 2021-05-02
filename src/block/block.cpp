#include "../../include/block/block.h"

std::list<SymbolTableEntry*>
Block:: getEntries() const {
    return entries;
}

void
Block:: activate() {
    for(auto entry : entries) 
        entry->setActive(true);
}

void
Block:: deactivate() {
    for(auto entry : entries)
        entry->setActive(false);
}   

SymbolTableEntry*
Block:: lookup(std::string id) {
    for(auto entry : entries) {
        if (entry->getId() == id && entry->isActive())
            return entry; 
    }

    return nullptr;
}

void 
Block:: insert(SymbolTableEntry* entry) {
    entries.push_back(entry);
}

std::ostream&
operator<<(std::ostream& os, const Block block) {
    for(auto entry : block.entries) 
        os << *entry << std::endl;

    return os;    
}