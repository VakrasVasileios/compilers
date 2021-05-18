#ifndef BOOLCONSTANT_H
#define BOOLCONSTANT_H

#include "constant.h"

/**
 * @brief A bool constant expression.
**/ 
class BoolConstant final : public Constant {
public:
    BoolConstant(bool value) : Constant(CONST_BOOL), value(value) {};
    ~BoolConstant() = default;
    /**
     * @brief Returns a read access to this BoolConstant value.
     * 
     * @return a read access to this BoolConstant value.
    **/ 
    bool      get_value() const;
protected:
    std::ostream&   LogExpression(std::ostream& os) const override;     
private:
    bool        value;   
    std::string value_to_string() const; 
};

#endif
