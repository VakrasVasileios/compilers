#include "../../../include/expression/function_call/function_call.h"

Function*
FunctionCall:: get_called_function() const {
    return called_function;
}

std::list<Expression*>
FunctionCall:: get_params() const {
    return params;
}

void
FunctionCall:: IncludeParameter(Expression* param) {
    params.push_back(param);
}

std::ostream&
FunctionCall:: LogExpression(std::ostream& os) const {
    return os << called_function->get_id();
}