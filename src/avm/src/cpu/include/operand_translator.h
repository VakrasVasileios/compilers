#ifndef OPERAND_TRANSLATOR_H
#define OPERAND_TRANSLATOR_H

#include "../../../../../util/contract/include/contract.h"
#include "../../../../target_code/include/vm_arg.h"
#include "../../memcell/include/avm_memcell.h"
#include "avm_cpu.h"

namespace avm
{
/**
 * @brief Translates a vmarg operand into an avm memcell.
 * 
 */
class OperandTranslator final : public target_code::VmargVisitor {
public:
    /**
     * @brief Constructs a new OperandTranslator object.
     * 
     * @param register the avm cpu register on which to load
     * the avm memcell in case of a constant vmarg.
     * 
     */
    OperandTranslator(memcell::AvmMemcell* _register) : register_(_register) {}
    /**
     * @brief Destroys this OperandTranslator object.
     * 
     */
    ~OperandTranslator() = default;

    void VisitGlobalVmarg(target_code::GlobalVmarg* arg) override;

    void VisitLocalVmarg(target_code::LocalVmarg* arg) override;

    void VisitFormalVmarg(target_code::FormalVmarg* arg) override;

    void VisitBoolVmarg(target_code::BoolVmarg* arg) override;

    void VisitStringVmarg(target_code::StringVmarg* arg) override;

    void VisitNumberVmarg(target_code::NumberVmarg* arg) override;

    void VisitNilVmarg(target_code::NilVmarg* arg) override;

    void VisitLibFuncVmarg(target_code::LibFuncVmarg* arg) override;

    void VisitUserFuncVmarg(target_code::UserFuncVmarg* arg) override;

    void VisitLabelVmarg(target_code::LabelVmarg* arg) override;

    void VisitRetValVmarg(target_code::RetValVmarg* arg) override;
    /**
     * @brief Returns a read access to this OperandTranslator result.
     * 
     * @return a read access to this OperandTranslator result, not null 
     */
    memcell::AvmMemcell* result() const;
private:
    memcell::AvmMemcell* result_;
    memcell::AvmMemcell* register_;
};
}

#endif