#ifndef NUMERICCONSTANT_H
#define NUMERICCONSTANT_H

#include "constant.h"

/**
 * @brief A numeric constant expression.
**/ 
class NumericConstant: public Constant {
protected:
    NumericConstant() : Constant(CONST_NUM) {};
    ~NumericConstant() = default;
};

#endif
