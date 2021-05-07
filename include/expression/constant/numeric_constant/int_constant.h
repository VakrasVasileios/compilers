#ifndef INTCONSTANT_H
#define INTCONSTANT_H

#include "numeric_constant.h"

class IntConstant final : public NumericConstant {
public:
    IntConstant(int value) : NumericConstant(), value(value) {};
    ~IntConstant() = default;
protected:
    std::ostream&   LogExpression(std::ostream& os) const override;     
private:
    int value;    
};

#endif
