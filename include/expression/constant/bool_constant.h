#ifndef BOOLCONSTANT_H
#define BOOLCONSTANT_H

#include "constant.h"

class BoolConstant final : public Constant {
public:
    BoolConstant(bool value) : Constant(CONST_BOOL), value(value) {};
    ~BoolConstant() = default;
protected:
    std::ostream&   LogExpression(std::ostream& os) const override;     
private:
    bool value;    
};

#endif
