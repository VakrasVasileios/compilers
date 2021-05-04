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
        if (entry->get_type() == VAR) // deactivate variables only
            entry->set_active(false);
    }
}   

SymbolTableEntry*
Block:: Lookup(std::string id) {
    for(auto entry : entries) {
        if (entry->get_id() == id) 
            return entry;
            // if (entry->is_active())
            //     return entry;
            // else {
            //     if(log_flag)
            //         std::cout << "Error, in line: " << lineno << ". Cannot access " << id  << ", peviously defined in line: " << entry->get_line() << std::endl;
            //     return nullptr;   
    }

    return nullptr;
}

bool IsAFUnctionEntry(SymbolTableEntry* entry) {
    return entry->get_type() == LIB_FUNC || entry->get_type() == USER_FUNC;
}

SymbolTableEntry*
Block:: LookupFunc(std::string id) {
    for(auto entry : entries) {
        if (entry->get_id() == id && (IsAFUnctionEntry(entry))) {
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
    for(auto entry : block.entries) {
        entry->Log(os);
        os << "\n";
    }
    return os;    
}