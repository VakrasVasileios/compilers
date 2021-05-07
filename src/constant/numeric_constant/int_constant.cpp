#include "../../../include/constant/numeric_constant/int_constant.h"

std::ostream&
IntConstant:: do_print(std::ostream& os) const {
    return os << value;
}