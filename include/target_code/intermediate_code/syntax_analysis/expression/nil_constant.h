#ifndef NILCONSTANT_H
#define NILCONSTANT_H

#include "constant.h"

namespace expression {
    /**
     * @brief A NILL constant expression.
    **/ 
    class NilConstant final : public Constant {
    public:
        NilConstant(std::nullptr_t value) : Constant(CONST_NIL), value(value) {};
        ~NilConstant() = default;
        /**
         * @brief Returns a read access to this NilConstant value.
         * 
         * @return a read access to this NilConstant value, null
         * 
        **/ 
        std::nullptr_t  get_value() const;
        std::string     to_string() const override;
        unsigned int    to_unsigned() const override;
        void            Accept(ExpressionVisitor* expr_visitor) override;
    protected:
        std::ostream&   LogExpression(std::ostream& os) const override;    
    private:
        std::nullptr_t  value;    
    };
}

#endif
