#ifndef CONSTANT_H
#define CONSTANT_H

#include "expression.h"
#include <string>
#include <ostream>

/*
Provides a constant expression with a type and a value.
*/
class Constant final : public Expression {
public:
    Constant() = default;
    /*
    The Parameterized Constructor of this Constant. Sets this Constant int value. 
    */
    Constant(int _num) : Expression(CONST_NUM), num(_num) {}
    /*
    The Parameterized Constructor of this Constant. Sets this Constant double value. 
    */
    Constant(double _num) : Expression(CONST_NUM), num(_num) {}
    /*
    The Parameterized Constructor of this Constant. Sets this Constant string value. 
    */
    Constant(const char* _str) : Expression(CONST_STR), str(std::string(_str)) {}
    /*
    The Parameterized Constructor of this Constant. Sets this Constant boolean value. 
    */
    Constant(bool _val) : Expression(CONST_BOOL), boolVal(_val) {}

    Constant(std::nullptr_t _nil) : Expression(NIL_t), _NIL(_nil) {}
    /*
    Returns a read access to this Constant value.
    */
    template<typename typ>
    typ                  get_value(void) const;

    void                 LogExpr(std::ostream& output);
private:
    union {
        double          num;
        std::string     str;
        bool            boolVal;
        std::nullptr_t  _NIL;
    };

    double getDouble() { return num; }
    std::string getStr() {return str; }
    bool getBool() {return boolVal; }
    std::nullptr_t getNil() { return nullptr; }
};

#endif