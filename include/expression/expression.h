#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

/**
 * @brief The type of an expression.
 * 
**/ 
enum ExprType {
    LIB_FUNC,   
    USER_FUNC, 
    VAR,        

    CONST_NUM,  
    CONST_BOOL, 
    CONST_STR,  
    CONST_NIL,  

    FUNCTION_CALL,

    TABLE_ITEM,
    TABLE_MAKE
};

/**
 * @brief An expression.
 * 
**/ 
class Expression {
public:
    /**
     * @brief Returns a read access to this Expression type.
     * 
     * @return a read access to this Expression type.
    **/ 
    ExprType                get_type() const;
   
    friend std::ostream&    operator << (std::ostream& os, const Expression* expression);
protected:
    Expression(ExprType _type) : type(_type) {};
    ~Expression() = default;
    /**
     * @brief Logs this Expression.
     * 
     * @param os the ostream to output this Expression.
     * 
    **/ 
    virtual std::ostream&   LogExpression(std::ostream& os) const = 0;
private:
    ExprType type;
};

#endif
