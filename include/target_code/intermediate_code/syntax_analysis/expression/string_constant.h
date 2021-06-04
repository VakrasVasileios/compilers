#ifndef STRINGCONSTANT_H
#define STRINGCONSTANT_H

#include "constant.h"

namespace expression {
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
        std::string     to_string() const override;
        unsigned int    to_unsigned() const override;
    protected:
        std::ostream&   LogExpression(std::ostream& os) const override; 
    private:
        std::string     value;
    };
}

#endif
