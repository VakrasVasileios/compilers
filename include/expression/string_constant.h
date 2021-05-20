#ifndef STRINGCONSTANT_H
#define STRINGCONSTANT_H

#include "constant.h"

/**
 * @brief A string constant expression.
**/ 
class StringConstant final : public Constant {
public:
    StringConstant(std::string value) : Constant(CONST_STR), value(value) {};
    ~StringConstant() = default;
    /**
     * @brief Returns a read access to this StringConstant value.
    **/ 
    std::string     get_value() const;
protected:
    std::ostream&   LogExpression(std::ostream& os) const override; 
private:
    std::string     value;
};

#endif
