#ifndef CONSTANT_H
#define CONSTANT_H

#include "expression.h"
#include <string>
#include <ostream>

/*
Provides a constant expression.
*/
class Constant : public Expression {
protected:
    Constant(ExprType type) : Expression(type){};
    ~Constant() = default;         
};

#endif