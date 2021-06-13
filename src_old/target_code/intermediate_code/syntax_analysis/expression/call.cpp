#include "../../../../../include/target_code/intermediate_code/syntax_analysis/expression/call.h"

namespace expression {
    Symbol*
    Call:: get_called_symbol() const {
        return called_symbol;
    }

    CallSuffix* 
    Call::get_call_suffix() const {
        return call_suffix;
    }

    Symbol*
    Call:: get_ret_val() const {
        return ret_val;
    }

    std::string 
    Call::to_string() const {
        return ret_val->to_string();
    }

    unsigned int
    Call::to_unsigned() const {
        return 0; 
    }

    void
    Call::Accept(ExpressionVisitor* expr_visitor) {
        PRECONDITION(expr_visitor != nullptr);
        expr_visitor->VisitCall(this);    
    }

    std::ostream&
    Call:: LogExpression(std::ostream& os) const {
        return os << ret_val->get_id();
    }

    Symbol*
    Call:: verify_called_symbol(Symbol* called_symbol) {
        PRECONDITION(called_symbol != nullptr);
        return called_symbol;
    }

    CallSuffix* 
    Call::verify_call_suffix(CallSuffix* call_suffix) {
        PRECONDITION(call_suffix != nullptr);
        return call_suffix;
    }

    Symbol* 
    Call:: verify_ret_val(Symbol* ret_val) {
        PRECONDITION(ret_val != nullptr);
        return ret_val;
    }
}
