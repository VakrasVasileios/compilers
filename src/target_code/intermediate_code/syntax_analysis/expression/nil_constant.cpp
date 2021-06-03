#include "../../../../../include/target_code/intermediate_code/syntax_analysis/expression/nil_constant.h"

namespace expression {
    std::nullptr_t
    NilConstant:: get_value() const {
        return value;
    }

    std::string 
    NilConstant:: to_string() const {
        return "NIL";
    }

    unsigned int
    NilConstant::to_unsigned() const {
        return 0;
    }

    std::ostream&
    NilConstant:: LogExpression(std::ostream& os) const {
        return os << to_string();
    }
}