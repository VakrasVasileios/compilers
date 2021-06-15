#include "../include/operand_translator.h"

namespace avm
{

void 
OperandTranslator::VisitGlobalVmarg(target_code::GlobalVmarg* arg) {
    PRECONDITION(arg != nullptr);
    result_ = avm_stack.environment(*arg);
}

void 
OperandTranslator::VisitLocalVmarg(target_code::LocalVmarg* arg) {
    PRECONDITION(arg != nullptr);
    result_ = avm_stack.environment(*arg);
}

void 
OperandTranslator::VisitFormalVmarg(target_code::FormalVmarg* arg) {
    PRECONDITION(arg != nullptr);
    result_ = avm_stack.environment(*arg);
}

void 
OperandTranslator::VisitBoolVmarg(target_code::BoolVmarg* arg) {
    PRECONDITION(arg != nullptr);
    register_ = new memcell::BoolMemcell(arg->get_value());
    result_ = register_;
}

void 
OperandTranslator::VisitStringVmarg(target_code::StringVmarg* arg) {
    PRECONDITION(arg != nullptr);
    register_ = 
        new memcell::StringMemcell(constants.GetString(arg->get_value()));
    result_ = register_;
}

void 
OperandTranslator::VisitNumberVmarg(target_code::NumberVmarg* arg) {
    PRECONDITION(arg != nullptr);
    register_ = new memcell::NumMemcell(constants.GetNumber(arg->get_value()));
    result_ = register_;
}

void 
OperandTranslator::VisitNilVmarg(target_code::NilVmarg* arg) {
    PRECONDITION(arg != nullptr);
    register_ = new memcell::NilMemcell();
    result_ = register_;
}

void 
OperandTranslator::VisitLibFuncVmarg(target_code::LibFuncVmarg* arg) {
   PRECONDITION(arg != nullptr); 
   register_ = 
    new memcell::LibfuncMemcell(constants.GetLibfunc(arg->get_value()));
   result_ = register_;
}

void 
OperandTranslator::VisitUserFuncVmarg(target_code::UserFuncVmarg* arg) {
    PRECONDITION(arg != nullptr);
    register_ = 
        new memcell::UserfuncMemcell(
            constants.GetUserfunc(arg->get_value()).taddress);
    result_ = register_;        
}

void 
OperandTranslator::VisitLabelVmarg(target_code::LabelVmarg* arg) {
    PRECONDITION(arg != nullptr);
    register_ = new memcell::NumMemcell(arg->get_value());
    result_ = register_;
}

void 
OperandTranslator::VisitRetValVmarg(target_code::RetValVmarg* arg) {
    PRECONDITION(arg != nullptr);
    register_ = new memcell::NumMemcell(arg->get_value());
    result_ = register_;
}

memcell::AvmMemcell* 
OperandTranslator::result() const {
    POSTCONDITION(result_ != nullptr);
    return result_;
}

}