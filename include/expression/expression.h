#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

enum ExprType {
    LIB_FUNC,
    USER_FUNC,

    CONST_NUM,
    CONST_BOOL,
    CONST_STR,
    
    NIL_t,

    ARITH_EXPR,
    BOOL_EXPR,
    ASSIGN_EXPR,
    NEWTABLE_EXPR,
    VAR,
    TABLE_ITEM
};

/*
Provides an expression.
*/
class Expression {
public:
    /*
    The Paramterized Constructor of this Expression. 
    */
    Expression(ExprType _type) : type(_type) {};
    ~Expression() = default;
    /*
    Returns a read access to this Expression type.
    */
    ExprType        get_type() const;

    virtual void    LogExpr(std::ostream& output) {};
protected:
    /*
    Returns a read access to this Expression type as a string.
    */
    std::string     TypeToString(void) const;
private:
    ExprType type;
};

#endif
