#include "../../include/expression/symbol.h"
#include <assert.h>

namespace expression {
    std::string
    Symbol:: get_id() const {
        return id;
    }

    unsigned int
    Symbol:: get_line() const {
        return line;
    }

    unsigned int
    Symbol:: get_scope() const {
        return scope;
    }

    bool
    Symbol:: is_active() const {
        return active;
    }

    ScopeSpace
    Symbol:: get_space() const {
        return space;
    }

    unsigned int
    Symbol:: get_offset() const {
        return offset;
    }

    std::list<Symbol*>
    Symbol:: get_formal_arguments() const {
        return formal_arguments;
    }

    void
    Symbol:: set_active(bool active) {
        this->active = active;
    }

    bool
    Symbol:: HasFormalArg(Symbol* formal_arg) {
        PRECONDITION(formal_arg != nullptr);
        PRECONDITION(formal_arg->get_space() == FORMAL_ARG);
        std::string wanted = formal_arg->get_id();
        for (auto i : formal_arguments) {
            if (i->get_id() == wanted) {
                return true;
            }
        }

        return false;  
    }

    Expression* 
    Symbol:: get_index() const {
        PRECONDITION(get_type() == TABLE_ITEM);
        return index;
    }

    void
    Symbol:: AddFormalArg(Symbol* formal_arg) {
        PRECONDITION(!HasFormalArg(formal_arg));
        formal_arguments.push_back(formal_arg);
        INVARIANT(is_state_valid());
    }

    std::string
    Symbol:: to_string() const {
        return id;
    }

    ExprType
    Symbol:: verify_type(ExprType type) {
        PRECONDITION(type == VAR || type == LIB_FUNC || type == USER_FUNC || type == TABLE_ITEM);
        return type;
    }

    std::string
    Symbol:: verify_id(std::string id) {
        PRECONDITION(!id.empty());
        return id;
    }

    unsigned int
    Symbol:: verify_line(unsigned int line) {
        PRECONDITION(line >= 0);
        return line;
    }

    unsigned int
    Symbol:: verify_scope(unsigned int scope) {
        PRECONDITION(scope >= 0);
        return scope;
    }

    unsigned int
    Symbol:: verify_offset(unsigned int offset) {
        PRECONDITION( offset >= 0);
        return offset;
    }

    Expression*
    Symbol:: verify_index(ExprType type, Expression* index) {
        PRECONDITION(index != nullptr? type == TABLE_ITEM : type != TABLE_ITEM);
        return index;
    }

    bool
    Symbol:: is_state_valid() {
        bool is_type_valid = get_type() == VAR || get_type() == LIB_FUNC || get_type() == USER_FUNC;
        bool is_id_valid = !id.empty();
        bool is_line_valid = line >= 0;
        bool is_scope_valid = scope >= 0;
        bool is_offset_valid = offset >= 0;
        bool is_index_valid = index != nullptr? get_type() == TABLE_ITEM : get_type() != TABLE_ITEM;

        return  is_type_valid && is_id_valid && is_line_valid &&
                is_scope_valid && is_offset_valid && is_index_valid;
    }

    std::string
    Symbol:: space_to_string() const {
        switch (space) {
            case PROGRAM_VAR:
                return "PROGRAM_VAR";
            case FUNCTION_LOCAL:
                return "FUNCTION_LOCAL";  
            case FORMAL_ARG:
                return "FORMAL_ARG";
            default:
                assert(false);        
        }
    }

    std::string 
    Symbol:: sym_to_string() const {
        switch (get_type()) {
            case LIB_FUNC:
                return "library function";
            case USER_FUNC:
                return "user function";
            case VAR:
                if (space == FORMAL_ARG)
                    return "formal variable";
                if (scope == 0)
                    return "global variable";
                else    
                    return "local variable";    
            default:
                assert(false);            
        }
    }

    std::ostream& operator<<(std::ostream& os, const Symbol* symbol) {
        return os   << "[" << symbol->sym_to_string() << "]"
                    << " \"" << symbol->get_id() << "\""
                    << " (line " << symbol->get_line() << ")" 
                    << " (scope " << symbol->get_scope() << ")";
    }

    std::ostream&
    Symbol:: LogExpression(std::ostream& os) const {
        return os << id;
    }
}