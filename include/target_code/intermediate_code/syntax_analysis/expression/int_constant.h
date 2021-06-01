#ifndef INTCONSTANT_H
#define INTCONSTANT_H

#include "numeric_constant.h"

namespace expression {
    /**
     * @brief An int constant expression.
     * 
    **/ 
    class IntConstant final : public NumericConstant {
    public:
        IntConstant(int value) : NumericConstant(CONST_INT), value(value) {};
        ~IntConstant() = default;
        /**
         * @brief Returns a read access to this IntConstant value.
         * 
         * @return a read access to this IntConstant value
        **/ 
        int             get_value() const;
        std::string     to_string() const override;
    protected:
        std::ostream&   LogExpression(std::ostream& os) const override;     
    private:
        int value;    
    };
}

#endif
