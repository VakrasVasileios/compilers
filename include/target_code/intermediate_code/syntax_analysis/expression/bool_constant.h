#ifndef BOOLCONSTANT_H
#define BOOLCONSTANT_H

#include "constant.h"

namespace expression {
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
        bool            get_value() const;
        std::string     to_string() const override;
        unsigned int    to_unsigned() const override;
    protected:
        std::ostream&   LogExpression(std::ostream& os) const override;     
    private:
        bool            value;   
    };
}

#endif
