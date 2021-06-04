#ifndef DOUBLECONSTANT_H
#define DOUBLECONSTANT_H

#include "numeric_constant.h"

namespace expression {
    /**
     * @brief A double constant.
    **/ 
    class DoubleConstant final : public NumericConstant {
    public:
        DoubleConstant(double value) : NumericConstant(CONST_DOUBLE), value(value) {};
        ~DoubleConstant() = default;
        /**
         * @brief Returns a read access to this DoubleConstant value.
         * 
         * @return a read access to this DoubleConstant value
        **/ 
        double          get_value() const;
        std::string     to_string() const override;
        unsigned int    to_unsigned() const override;
        void            Accept(ExpressionVisitor* expr_visitor) override;
    protected:
        std::ostream&   LogExpression(std::ostream& os) const override;     
    private:
        double value;    
    };
}

#endif
