#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

enum ExprType {
    LIB_FUNC,
    USER_FUNC,
    VAR,

    CONST_NUM,
    CONST_BOOL,
    CONST_STR,
    CONST_NIL,

    ARITH_EXPR,
    BOOL_EXPR,
    ASSIGN_EXPR,
    NEWTABLE_EXPR,
    TABLE_ITEM
};

class Expression {
public:
    ExprType                get_type() const;
   
    friend std::ostream&    operator << (std::ostream& os, const Expression* expression);
protected:
    Expression(ExprType _type) : type(_type) {};
    ~Expression() = default;

    std::string             TypeToString(void) const;

    virtual std::ostream&   LogExpression(std::ostream&) const = 0;
private:
    ExprType type;
};

#endif
