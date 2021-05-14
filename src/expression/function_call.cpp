#include "../../include/expression/function_call.h"

Symbol*
FunctionCall:: get_called_symbol() const {
    return called_symbol;
}

std::list<Expression*>
FunctionCall:: get_params() const {
    return params;
}

void
FunctionCall:: IncludeParameter(Expression* param) {
    params.push_back(param);
}

std::string 
FunctionCall:: get_ret_val() const {
    return ret_val;
}

void
FunctionCall:: set_ret_val(const std::string ret_val) {
    this->ret_val = ret_val; 
}

std::ostream&
FunctionCall:: LogExpression(std::ostream& os) const {
    return os << ret_val;
}