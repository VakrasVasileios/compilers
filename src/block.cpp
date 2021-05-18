#include "../include/block.h"
#include <iostream>


std::list<Symbol*>
Block:: get_symbols() const {
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

bool
Block:: IsAFunctionEntry(Symbol* entry) {
    return entry->get_type() == LIB_FUNC || entry->get_type() == USER_FUNC;
}

bool
Block:: IsHiddenVariable(Symbol* entry) {
    return entry->get_id().at(0) == '^';
}

void 
Block:: Insert(Symbol* entry) {
    entries.push_back(entry);
}

bool
Block:: ContainsOnlyHiddenVars() {
    unsigned int hidden_var_count = 0;
    for (auto entry : entries) {
        if (IsHiddenVariable(entry))
            hidden_var_count++;
    } 

    return hidden_var_count == entries.size();
}

std::ostream&
operator<<(std::ostream& os, const Block block) {
    #if !defined TEST
        for(auto entry : block.entries) {
            os << entry << "\n";
        }
    #else 
        for(auto entry : block.entries) {
            if (!block.IsHiddenVariable(entry))
            {
                os << entry << "\n";
            }
        }
    #endif


    
    return os;    
}