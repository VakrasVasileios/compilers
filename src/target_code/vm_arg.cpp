#include "../../include/target_code/vm_arg.h"

namespace target_code {
    std::ostream 
    &operator<<(std::ostream &os, const vmarg &rhs) {
        os << rhs.value;
        return os;
    }

    std::string
    type_to_string(Vmarg_t _type) {
        switch (_type) {
            case GLOBAL_a: return "global_a";
            case LOCAL_a: return "local_a";
            case FORMAL_a: return "formal_a";
            case BOOL_a: return "bool_a";
            case STRING_a: return "string_a";
            case NUMBER_a: return "number_a";
            case NIL_a: return "nil_a";
            case LIBFUNC_a: return "libfunc_a";
            case USERFUNC_a: return "userfunc_a";
            case LABEL_a: return "label_a";
            case RETVAL_a: return "retval_a";        
            default: assert(false);
        }
    }
}