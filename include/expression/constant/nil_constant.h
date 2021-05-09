#ifndef NILCONSTANT_H
#define NILCONSTANT_H

#include "constant.h"

/*
Provides a nill constant expression.
*/
class NilConstant final : public Constant {
public:
    NilConstant(std::nullptr_t value) : Constant(CONST_NIL), value(value) {};
    ~NilConstant() = default;
protected:
    std::ostream&   LogExpression(std::ostream& os) const override;    
private:
    std::nullptr_t value;    
};

#endif
