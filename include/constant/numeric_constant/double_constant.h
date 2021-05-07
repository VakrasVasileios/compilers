#ifndef DOUBLECONSTANT_H
#define DOUBLECONSTANT_H

#include "numeric_constant.h"

/*
Provides a constant of double type.
*/
class DoubleConstant final : public NumericConstant {
public:
    DoubleConstant(double value) : NumericConstant(), value(value) {};
    ~DoubleConstant() = default;
protected:
    std::ostream&   do_print(std::ostream& os) const override;     
private:
    double value;    
};

#endif
