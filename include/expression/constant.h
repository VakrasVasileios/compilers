#ifndef CONSTANT_H
#define CONSTANT_H

#include "primary.h"
#include <string>
#include <ostream>

/**
 * @brief A constant primary expression.
**/ 
class Constant : public Primary {
protected:
    Constant(ExprType type) : Primary(type){};
    ~Constant() = default;         
};

#endif