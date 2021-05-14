#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

/*
Provides an expression type.
*/
enum ExprType {
    LIB_FUNC,   //library function symbol.
    USER_FUNC,  //user function symbol.
    VAR,        //variable symbol.

    CONST_NUM,  //numeric constant.
    CONST_BOOL, //boolean constant.
    CONST_STR,  //string constant.
    CONST_NIL,  //NIL constant.

    ARITH_EXPR,
    BOOL_EXPR,
    ASSIGN_EXPR,
    NEWTABLE_EXPR,
    TABLE_ITEM,

    FUNCTION_DEF,
    FUNCTION_CALL
};

/*
Provides an expression.
*/
class Expression {
public:
    /*
    Returns a read access to this Expression type.
    */
    ExprType                get_type() const;
   
    friend std::ostream&    operator << (std::ostream& os, const Expression* expression);
protected:
    Expression(ExprType _type) : type(_type) {};
    ~Expression() = default;
    /*
    Returns a read access to this Expression type as a string.
    */
    std::string             TypeToString(void) const;
    /*
    Logs this Expression.
    */
    virtual std::ostream&   LogExpression(std::ostream&) const = 0;
private:
    ExprType type;
};

#endif
