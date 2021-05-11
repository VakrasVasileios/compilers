#ifndef FUNCTIONCALL_H
#define FUNCTIONCALL_H

#include "../expression.h"
#include "../symbol/function/function.h"
#include <list>

/*
Provides a function call that always resides at the top
call stack frame. 
*/
class FunctionCall : public Expression {
public:
    FunctionCall(Function* called_function, std::list<Expression*> params)
    : Expression(FUNCTION_CALL), called_function(called_function), params(params) {};

    ~FunctionCall() = default;
    /*
    Returns this FunctionCall called function.
    */
    Function*               get_called_function() const;
    /*
    Returns this FunctionCall passed parameters list.
    */
    std::list<Expression*>  get_params() const;
    /*
    Includes a parameter to this FunctionCall.
    */
    void                    IncludeParameter(Expression* param);
    /*
    Returns a read access to this FunctionCall return value.
    */
    std::string             get_ret_val() const;
    /*
    Sets this FunctionCall return value.
    */
    void                    set_ret_val(const std::string ret_val);
protected:
    std::ostream&           LogExpression(std::ostream& os) const override;
private:
    Function*               called_function; 
    std::list<Expression*>  params;  
    std::string             ret_val;
};

#endif