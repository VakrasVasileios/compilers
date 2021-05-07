#ifndef STRINGCONSTANT_H
#define STRINGCONSTANT_H

#include "constant.h"

/*
Provides a constant of string type.
*/
class StringConstant final : public Constant {
public:
    StringConstant(std::string value) : Constant(CONST_STR), value(value) {};
    ~StringConstant() = default;
protected:
    std::ostream&   do_print(std::ostream& os) const override; 
private:
    std::string value;
};

#endif
