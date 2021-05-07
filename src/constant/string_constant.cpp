#include "../../include/constant/string_constant.h"

std::ostream&
StringConstant:: do_print(std::ostream& os) const {
    return os << value;
}