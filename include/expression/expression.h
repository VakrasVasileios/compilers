#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

enum ExprType {
    /*
    The type of a Library Function.
    */
    LIB_FUNC = 0,
    /*
    The type of a User Function.
    */
    USER_FUNC = 1,
    /*
    The type of a Constant.
    */
    CONST = 2,
    /*
    The type of a Variable.
    */
    VAR = 3
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
protected:
    /*
    Returns a read access to this Expression type as a string.
    */
    std::string     TypeToString(void) const;
private:
    ExprType type;    
};

#endif
