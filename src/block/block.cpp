#include "../../include/block/block.h"
#include <iostream>


std::list<SymbolTableEntry*>
Block:: get_entries() const {
    return entries;
}

void
Block:: Activate() {
    for(auto entry : entries) 
        entry->set_active(true);
}

void
Block:: Deactivate() {
    for(auto entry : entries) {
        if (entry->get_type() > 1) // deactivate variables only
            entry->set_active(false);
    }
}   

SymbolTableEntry*
Block:: Lookup(std::string id) {
    for(auto entry : entries) {
        if (entry->get_id() == id) {
            if (entry->is_active())
                return entry;
            else
                std::cout << "Cannot access " << id  << ", in line: " << entry->get_line() << std::endl;
        }
    }

    return nullptr;
}

SymbolTableEntry*
Block:: LookupFunc(std::string id) {
    for(auto entry : entries) {
        if (entry->get_id() == id) {
            if (entry->is_active() && entry->get_type() < CONST)
                return entry;
        }
    }

    return nullptr;
}

void 
Block:: Insert(SymbolTableEntry* entry) {
    entries.push_back(entry);
}

std::ostream&
operator<<(std::ostream& os, const Block block) {
    for(auto entry : block.entries) 
        os << *entry << std::endl;

    return os;    
}