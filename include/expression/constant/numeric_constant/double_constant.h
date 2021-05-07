#ifndef DOUBLECONSTANT_H
#define DOUBLECONSTANT_H

#include "numeric_constant.h"

class DoubleConstant final : public NumericConstant {
public:
    DoubleConstant(double value) : NumericConstant(), value(value) {};
    ~DoubleConstant() = default;
protected:
    std::ostream&   LogExpression(std::ostream& os) const override;     
private:
    double value;    
};

#endif
