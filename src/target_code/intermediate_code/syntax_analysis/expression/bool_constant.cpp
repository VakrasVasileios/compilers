#include "../../../../../include/target_code/intermediate_code/syntax_analysis/expression/bool_constant.h"

namespace expression {
    bool
    BoolConstant:: get_value() const {
        return value;
    }

    std::string     
    BoolConstant::to_string() const {
        if (value)
            return "\'true\'";
        else
            return "\'false\'";   
    }

    unsigned int
    BoolConstant::to_unsigned() const {
        return 0;    
    }

    std::ostream&
    BoolConstant:: LogExpression(std::ostream& os) const {
        return os << to_string();
    }
}