#include "../../../../../include/target_code/intermediate_code/syntax_analysis/expression/int_constant.h"

namespace expression {
    int 
    IntConstant:: get_value() const {
        return value;
    }

    std::string
    IntConstant::to_string() const {
        return std::to_string(value);
    }

    unsigned int    
    IntConstant::to_unsigned() const {
        return value;
    }

    std::ostream&
    IntConstant:: LogExpression(std::ostream& os) const {
        return os << value;
    }
}