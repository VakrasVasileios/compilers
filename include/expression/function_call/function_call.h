#ifndef FUNCTIONCALL_H
#define FUNCTIONCALL_H

#include "../expression.h"
#include "../symbol/symbol.h"
#include <list>

/*
Provides a function call that always resides at the top
call stack frame. 
*/
class FunctionCall final : public Expression {
public:
    FunctionCall(Symbol* called_symbol, std::list<Expression*> params)
    : Expression(FUNCTION_CALL), called_symbol(called_symbol), params(params) {};

    ~FunctionCall() = default;
    /*
    Returns this FunctionCall called symbol.
    */
    Symbol*                get_called_symbol() const;
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
    Symbol*                 called_symbol; 
    std::list<Expression*>  params;  
    std::string             ret_val;
};

#endif