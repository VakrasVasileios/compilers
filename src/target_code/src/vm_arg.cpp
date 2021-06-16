#include "../include/vm_arg.h"

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

void        
GlobalVmarg::Accept(VmargVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitGlobalVmarg(this);
}

uint8_t     
LocalVmarg::get_type() const {
    return LOCAL_a;
}

void        
LocalVmarg::Accept(VmargVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitLocalVmarg(this);    
}

uint8_t     
FormalVmarg::get_type() const {
    return FORMAL_a;
}

void        
FormalVmarg::Accept(VmargVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitFormalVmarg(this);    
}

uint8_t     
BoolVmarg::get_type() const {
    return BOOL_a;
}

void        
BoolVmarg::Accept(VmargVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitBoolVmarg(this);    
}

uint8_t     
StringVmarg::get_type() const {
    return STRING_a;
}

void        
StringVmarg::Accept(VmargVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitStringVmarg(this);    
}

uint8_t     
NumberVmarg::get_type() const {
    return NUMBER_a;
}

void        
NumberVmarg::Accept(VmargVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitNumberVmarg(this);    
}

uint8_t     
NilVmarg::get_type() const {
    return NIL_a;    
}

void        
NilVmarg::Accept(VmargVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitNilVmarg(this);    
}

uint8_t     
LibFuncVmarg::get_type() const {
    return LIBFUNC_a;    
}

void        
LibFuncVmarg::Accept(VmargVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitLibFuncVmarg(this);    
}

uint8_t     
UserFuncVmarg::get_type() const {
    return USERFUNC_a;    
}

void       
UserFuncVmarg::Accept(VmargVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitUserFuncVmarg(this);    
}

uint8_t     
LabelVmarg::get_type() const {
    return LABEL_a;    
}

void        
LabelVmarg::Accept(VmargVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitLabelVmarg(this);    
}

uint8_t     
RetValVmarg::get_type() const {
    return RETVAL_a;    
}

void        
RetValVmarg::Accept(VmargVisitor* visitor) {
    PRECONDITION(visitor != nullptr);
    visitor->VisitRetValVmarg(this);    
}

}