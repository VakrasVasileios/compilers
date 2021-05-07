#ifndef NUMERICCONSTANT_H
#define NUMERICCONSTANT_H

#include "../constant.h"

/*
Provides a constant of a numeric type.
*/
class NumericConstant: public Constant {
protected:
    NumericConstant() : Constant(CONST_NUM) {};
    ~NumericConstant() = default;
};

#endif
