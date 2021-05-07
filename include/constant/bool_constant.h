#ifndef BOOLCONSTANT_H
#define BOOLCONSTANT_H

#include "constant.h"

/*
Provides a constant of boolean type.
*/
class BoolConstant final : public Constant {
public:
    BoolConstant(bool value) : Constant(CONST_BOOL), value(value) {};
    ~BoolConstant() = default;
protected:
    std::ostream&   do_print(std::ostream& os) const override;     
private:
    bool value;    
};

#endif
