#include "../include/vmarg_factory.h"

namespace target_code
{
Vmarg* create_vmarg(Vmarg_t type, unsigned int value) {
    switch (type) {
        case GLOBAL_a:
            return new GlobalVmarg(value);
        case LOCAL_a:
            return new LocalVmarg(value);
        case FORMAL_a:
            return new FormalVmarg(value);
        case BOOL_a:
            return new BoolVmarg(value);
        case STRING_a:
            return new StringVmarg(value);
        case NUMBER_a:
            return new NumberVmarg(value);
        case NIL_a:
            return new NilVmarg(value);
        case LIBFUNC_a:
            return new LibFuncVmarg(value);
        case USERFUNC_a:
            return new UserFuncVmarg(value);
        case LABEL_a:
            return new LabelVmarg(value);
        case RETVAL_a:
            return new RetValVmarg(value);
        default:
            PRECONDITION(false);                                    
    }  
}
} 