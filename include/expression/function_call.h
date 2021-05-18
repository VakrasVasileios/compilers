#ifndef FUNCTIONCALL_H
#define FUNCTIONCALL_H

#include "expression.h"
#include "symbol.h"
#include <list>

/**
 * @brief A function call expression.
**/ 
class FunctionCall final : public Expression {
public:
    FunctionCall(Symbol* called_symbol, std::list<Expression*> params)
    : Expression(FUNCTION_CALL), called_symbol(called_symbol), params(params) {};

    ~FunctionCall() = default;
    /**
     * @brief Returns a read access to this FunctionCall called symbol.
     * 
     * @return a read access to this FunctionCall called symbol, can be null
    **/ 
    Symbol*                get_called_symbol() const;
    /**
     * @brief Returns a read access to this FunctionCall parameters.
     * 
     * @return a read access to this FunctionCall parameters, not null
     * 
    **/ 
    std::list<Expression*>  get_params() const;
    /**
     * @brief Includes a parameter to this FunctionCall.
     * 
     * @param param the parameter to be included to this FunctionCall, can be null
    **/ 
    void                    IncludeParameter(Expression* param);
    /**
     * @brief Returns a read access to this FunctionCall return value.
     * 
     * @return a read access to this FunctionCall return value, can be null
    **/ 
    std::string             get_ret_val() const;
    /**
     * @brief Sets this FunctionCall return value.
     * 
     * @param ret_val the return value to set his FunctionCall return value, can be null
    **/ 
    void                    set_ret_val(const std::string ret_val);
protected:
    std::ostream&           LogExpression(std::ostream& os) const override;
private:
    Symbol*                 called_symbol; 
    std::list<Expression*>  params;  
    std::string             ret_val;
};

#endif