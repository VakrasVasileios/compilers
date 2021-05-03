#ifndef CONSTANT_H
#define CONSTANT_H

#include "expression.h"
#include <string>

/*
Provides a type of a constant expression.
*/
enum const_t {
    INT_T = 0,
    DOUBLE_T = 1,
    STRING_T = 2,
    BOOL_T = 3,
    NIL_T = 4
};

/*
Provides a constant expression with a type and a value.
*/
class Constant final : public Expression {
public:
    Constant() = default;
    /*
    The Parameterized Constructor of this Constant. Sets this Constant int value. 
    */
    Constant(int _num) : Expression(CONST), intNum(_num), type(INT_T) {}
    /*
    The Parameterized Constructor of this Constant. Sets this Constant double value. 
    */
    Constant(double _num) : Expression(CONST), doubleNum(_num), type(DOUBLE_T) {}
    /*
    The Parameterized Constructor of this Constant. Sets this Constant string value. 
    */
    Constant(const char* _str) : Expression(CONST), str(std::string(_str)), type(STRING_T) {}
    /*
    The Parameterized Constructor of this Constant. Sets this Constant boolean value. 
    */
    Constant(bool _val) : Expression(CONST),  boolVal(_val), type(BOOL_T) {}

    Constant(std::nullptr_t NIL) : Expression(CONST), NIL(NIL), type(NIL_T) {}
    /*
    Returns a read access to this Constant value.
    */
    template<typename typ>
    typ                  get_value(void) const;

private:
    const_t type;
    
    union {
        int             intNum;
        double          doubleNum;
        std::string     str;
        bool            boolVal;
        std::nullptr_t  NIL;
    };
};

#endif