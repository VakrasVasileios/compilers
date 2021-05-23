#ifndef CALL_H
#define CALL_H

#include "primary.h"
#include "symbol.h"
#include <list>

/**
 * @brief A call primary expression.
**/ 
class Call final : public Primary {
public:
    Call(Symbol* called_symbol)
    : Primary(CALL), called_symbol(called_symbol) {};

    ~Call() = default;
    /**
     * @brief Returns a read access to this Call called symbol.
     * 
     * @return a read access to this Call called symbol, can be null
    **/ 
    Symbol*                get_called_symbol() const;
    /**
     * @brief Returns a read access to this Call parameters.
     * 
     * @return a read access to this Call parameters, not null
     * 
    **/ 
    std::list<Expression*>  get_params() const;
    /**
     * @brief Includes a parameter to this Call.
     * 
     * @param param the parameter to be included to this Call, can be null
    **/ 
    void                    IncludeParameter(Expression* param);
    /**
     * @brief Returns a read access to this Call return value.
     * 
     * @return a read access to this Call return value, can be null
    **/ 
    Symbol*                 get_ret_val() const;
    /**
     * @brief Sets this Call return value.
     * 
     * @param ret_val the return value to set his Call return value, can be null
    **/ 
    void                    set_ret_val(Symbol* ret_val);
    
    std::string             to_string() const override;
protected:
    std::ostream&           LogExpression(std::ostream& os) const override;
private:
    Symbol*                 called_symbol; 
    std::list<Expression*>  params;  
    Symbol*                 ret_val;
};

#endif