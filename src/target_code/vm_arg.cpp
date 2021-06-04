#include "../../include/target_code/vm_arg.h"

namespace target_code {
    std::ostream 
    &operator<<(std::ostream &os, const vmarg &rhs) {
        os << rhs.value;
        return os;
    }
}