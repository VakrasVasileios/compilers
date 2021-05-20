#ifndef DOUBLECONSTANT_H
#define DOUBLECONSTANT_H

#include "numeric_constant.h"

/**
 * @brief A double constant.
**/ 
class DoubleConstant final : public NumericConstant {
public:
    DoubleConstant(double value) : NumericConstant(), value(value) {};
    ~DoubleConstant() = default;
    /**
     * @brief Returns a read access to this DoubleConstant value.
     * 
     * @return a read access to this DoubleConstant value
    **/ 
    double          get_value() const;
    std::string     to_string() const override;
protected:
    std::ostream&   LogExpression(std::ostream& os) const override;     
private:
    double value;    
};

#endif
