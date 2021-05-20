#include "../../include/expression/call.h"

Symbol*
Call:: get_called_symbol() const {
    return called_symbol;
}

std::list<Expression*>
Call:: get_params() const {
    return params;
}

void
Call:: IncludeParameter(Expression* param) {
    params.push_back(param);
}

std::string 
Call:: get_ret_val() const {
    return ret_val;
}

void
Call:: set_ret_val(const std::string ret_val) {
    this->ret_val = ret_val; 
}

std::ostream&
Call:: LogExpression(std::ostream& os) const {
    return os << ret_val;
}