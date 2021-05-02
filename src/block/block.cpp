<<<<<<< HEAD:src/block/Block.cpp
#include "../../include/block/Block.h"
#include <iostream>
=======
#include "../../include/block/block.h"
>>>>>>> 7749cc3c1c45aae7cd02cb3ffc4b238b0bcff3dd:src/block/block.cpp

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
    for(auto entry : entries)
        entry->set_active(false);
}   

SymbolTableEntry*
Block:: Lookup(std::string id) {
    for(auto entry : entries) {
<<<<<<< HEAD:src/block/Block.cpp
        if (entry->getId() == id)
            if (entry->isActive())
                return entry;
            else
                std::cout << "Cannot access " << id << std::endl;
=======
        if (entry->get_id() == id && entry->is_active())
            return entry; 
>>>>>>> 7749cc3c1c45aae7cd02cb3ffc4b238b0bcff3dd:src/block/block.cpp
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