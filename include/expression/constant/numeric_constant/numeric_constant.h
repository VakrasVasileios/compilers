#ifndef NUMERICCONSTANT_H
#define NUMERICCONSTANT_H

#include "../constant.h"

class NumericConstant: public Constant {
protected:
    NumericConstant() : Constant(CONST_NUM) {};
    ~NumericConstant() = default;
};

#endif
