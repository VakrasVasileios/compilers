#include "../../../include/constant/numeric_constant/double_constant.h"

std::ostream&
DoubleConstant:: do_print(std::ostream& os) const {
    return os << value;
}