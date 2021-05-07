#include "../../include/constant/bool_constant.h"

std::ostream&
BoolConstant:: do_print(std::ostream& os) const {
    return os << value;
}