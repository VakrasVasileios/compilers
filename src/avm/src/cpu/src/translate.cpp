#include "translate.h"

namespace avm
{
    namespace 
    {

    class OperandTranslator final : public target_code::VmargVisitor {
    public:
        OperandTranslator(memcell::AvmMemcell* _register) 
        : register_(_register) {}
        ~OperandTranslator() = default;

        void VisitGlobalVmarg(target_code::GlobalVmarg* arg) override {
            assert(arg != nullptr);
            result_ = memory::stack_segment.environment(*arg);
        }
        
        void VisitLocalVmarg(target_code::LocalVmarg* arg) override {
            assert(arg != nullptr);
            result_ = memory::stack_segment.environment(*arg);
        }

        void VisitFormalVmarg(target_code::FormalVmarg* arg) override {
            assert(arg != nullptr);
            result_ = memory::stack_segment.environment(*arg);
        }

        void VisitBoolVmarg(target_code::BoolVmarg* arg) override {
            assert(arg != nullptr);
            register_ = new memcell::BoolMemcell(arg->get_value());
            result_ = register_;
        }

        void VisitStringVmarg(target_code::StringVmarg* arg) override {
            assert(arg != nullptr);
            register_ = 
            new memcell::StringMemcell(
                memory::Constants::GetInstance().GetString(arg->get_value()));
            result_ = register_;
        }

        void VisitNumberVmarg(target_code::NumberVmarg* arg) override {
            assert(arg != nullptr);
            register_ = new memcell::NumMemcell(
                memory::Constants::GetInstance().GetNumber(arg->get_value()));
            result_ = register_;
        }

        void VisitNilVmarg(target_code::NilVmarg* arg) override {
            assert(arg != nullptr);
            register_ = new memcell::NilMemcell();
            result_ = register_;
        }

        void VisitLibFuncVmarg(target_code::LibFuncVmarg* arg) override {
            assert(arg != nullptr); 
            register_ = 
                new memcell::LibfuncMemcell(
                memory::Constants::GetInstance().GetLibfunc(arg->get_value()));
            result_ = register_;
        }

        void VisitUserFuncVmarg(target_code::UserFuncVmarg* arg) override {
            assert(arg != nullptr);
            register_ = 
                new memcell::UserfuncMemcell(
                    memory::Constants::GetInstance()
                        .GetUserfunc(arg->get_value()).taddress);
            result_ = register_;  
        }

        void VisitLabelVmarg(target_code::LabelVmarg* arg) override {
            assert(arg != nullptr);
            register_ = new memcell::NumMemcell(arg->get_value());
            result_ = register_;
        }

        void VisitRetValVmarg(target_code::RetValVmarg* arg) override {
            assert(arg != nullptr);
            register_ = new memcell::NumMemcell(arg->get_value());
            result_ = register_;
        }

        memcell::AvmMemcell* result() const {
            assert(result_ != nullptr);
            return result_;
        }
    private:
        memcell::AvmMemcell* result_;
        memcell::AvmMemcell* register_;
    };

    }
    namespace cpu
    {

    memcell::AvmMemcell* translate_operand(target_code::Vmarg* vmarg,
        memcell::AvmMemcell* _register) 
    {
        PRECONDITION(vmarg != nullptr);
        OperandTranslator* translator = new OperandTranslator(_register);
        vmarg->Accept(translator);

        auto result = translator->result();
        POSTCONDITION(result != nullptr);

        return result;
    }
    }
}