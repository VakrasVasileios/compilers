#ifndef CONSTANT_H
#define CONSTANT_H

#include "../expression/expression.h"
#include <string>
#include <ostream>

/*
Provides a constant expression with a type and a value.
*/
class Constant : public Expression {
protected:
    Constant(ExprType type) : Expression(type){};
    ~Constant() = default;         
};

#endif