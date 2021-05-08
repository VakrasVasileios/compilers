#include "../../include/block/block.h"
#include <iostream>


std::list<Symbol*>
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
        if (entry->get_type() == VAR)
            entry->set_active(false);
    }
}   

Symbol*
Block:: Lookup(std::string id) {
    for(auto entry : entries) {
        if (entry->get_id() == id) 
            return entry;
    }

    return nullptr;
}

bool IsAFUnctionEntry(Symbol* entry) {
    return entry->get_type() == LIB_FUNC || entry->get_type() == USER_FUNC;
}

Symbol*
Block:: LookupFunc(std::string id) {
    for(auto entry : entries) {
        if (entry->get_id() == id && (IsAFUnctionEntry(entry))) {
            return entry;
        }
    }

    return nullptr;
}

Function*
Block:: LookupPreviousFunc() {
    for (std::list<Symbol* >::reverse_iterator it = entries.rbegin(); it != entries.rend(); ++it) {
        auto entry = (*it);
        if(entry->get_type() == USER_FUNC || (entry)->get_type() == LIB_FUNC)
            return static_cast<Function*>(entry);
    }

    return nullptr;
}

void 
Block:: Insert(Symbol* entry) {
    entries.push_back(entry);
}

std::ostream&
operator<<(std::ostream& os, const Block block) {
    for(auto entry : block.entries) {
        os << entry << "\n";
    }
    return os;    
}