#ifndef NILCONSTANT_H
#define NILCONSTANT_H

#include "constant.h"

/*
Provides a constant of NIL type.
*/
class NilConstant final : public Constant {
public:
    NilConstant(std::nullptr_t value) : Constant(NIL_t), value(value) {};
    ~NilConstant() = default;
protected:
    std::ostream&   do_print(std::ostream& os) const override;    
private:
    std::nullptr_t value;    
};

#endif
