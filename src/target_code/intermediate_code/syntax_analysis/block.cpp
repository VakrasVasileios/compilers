#include "../../../../include/target_code/intermediate_code/syntax_analysis/block.h"

namespace syntax_analysis {
    std::list<expression::Symbol*>
    Block:: get_symbols() const {
        return symbols;
    }

    void
    Block:: Activate() {
        for(auto entry : symbols) 
            entry->set_active(true);
    }

    void
    Block:: Deactivate() {
        for(auto entry : symbols) {
            if (entry->get_type() == expression::VAR)
                entry->set_active(false);
        }
    }   

    expression::Symbol*
    Block:: Lookup(std::string id) {
        for(auto entry : symbols) {
            if (entry->get_id() == id) 
                return entry;
        }

        return nullptr;
    }

    bool
    Block:: IsHiddenVariable(expression::Symbol* entry) {
        return entry->get_id().at(0) == '^';
    }

    void 
    Block:: Insert(expression::Symbol* entry) {
        PRECONDITION(entry != nullptr);
        symbols.push_back(entry);
    }

    bool
    Block:: ContainsOnlyHiddenVars() {
        unsigned int hidden_var_count = 0;
        for (auto entry : symbols) {
            if (IsHiddenVariable(entry))
                hidden_var_count++;
        } 

        return hidden_var_count == symbols.size();
    }

    std::ostream&
    operator<<(std::ostream& os, const Block block) {
        #if !defined TEST
            for(auto entry : block.symbols) {
                os << entry << "\n";
            }
        #else 
            for(auto entry : block.symbols) {
                if (!block.IsHiddenVariable(entry))
                {
                    os << entry << "\n";
                }
            }
        #endif

        return os;    
    }
}