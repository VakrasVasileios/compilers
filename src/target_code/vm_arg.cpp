#include "../../include/target_code/vm_arg.h"

namespace target_code 
{
std::ostream 
&operator<<(std::ostream &os, const Vmarg* vmarg) {
    PRECONDITION(vmarg != nullptr);
    os << vmarg->value;
    return os;
}

unsigned int
Vmarg::get_value() const {
    POSTCONDITION(value >= 0);
    return value;    
}

void
Vmarg::set_value(const unsigned int value) {
    INVARIANT(value >= 0);
    PRECONDITION(value >= 0);
    POSTCONDITION(value >= 0);
    this->value = value;
    INVARIANT(value >= 0);
}

unsigned int
Vmarg::VerifyValue(unsigned int value) const {
    PRECONDITION(value >= 0);
    return value;
}

uint8_t     
GlobalVmarg::get_type() const {
    return GLOBAL_a;    
}

uint8_t     
LocalVmarg::get_type() const {
    return LOCAL_a;
}

uint8_t     
FormalVmarg::get_type() const {
    return FORMAL_a;
}

uint8_t     
BoolVmarg::get_type() const {
    return BOOL_a;
}

uint8_t     
StringVmarg::get_type() const {
    return STRING_a;
}

uint8_t     
NumberVmarg::get_type() const {
    return NUMBER_a;
}

uint8_t     
NilVmarg::get_type() const {
    return NIL_a;    
}

uint8_t     
LibFuncVmarg::get_type() const {
    return LIBFUNC_a;    
}

uint8_t     
UserFuncVmarg::get_type() const {
    return USERFUNC_a;    
}

uint8_t     
LabelVmarg::get_type() const {
    return LABEL_a;    
}

uint8_t     
RetValVmarg::get_type() const {
    return RETVAL_a;    
}

}