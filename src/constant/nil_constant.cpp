#include "../../include/constant/nil_constant.h"

std::ostream&
NilConstant:: do_print(std::ostream& os) const {
    return os << "NIL";
}