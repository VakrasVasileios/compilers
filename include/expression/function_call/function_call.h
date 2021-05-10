// #ifndef FUNCTIONCALL_H
// #define FUNCTIONCALL_H

// #include "../expression.h"
// #include "../symbol/function/function.h"

// class FunctionCall : public Expression {
// public:
//     FunctionCall(Function* called_function, unsigned int params_count)
//     : Expression(FUNCTION_CALL), called_function(called_function), params_count(params_count) {};

//     ~FunctionCall() = default;

//     const Function*             get_called_function() const { return called_function; };
//     const unsigned int          get_params_count() const { return params_count; };
// private:
//     const Function* called_function; 
//     const unsigned int params_count = 0;   
// };

// #endif