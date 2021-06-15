#include "execute.h"
#include "translate.h"
#include "execute_function.h"

namespace avm
{
    namespace
    {
          
    class InstructionExecuter : public target_code::InstructionVisitor {
    public:
        InstructionExecuter() = default;
        ~InstructionExecuter() = default;

        void VisitAssign(target_code::Assign* inst) const override {
            assert(inst != nullptr);
        }

        void VisitAdd(target_code::Add* inst) const override {
            assert(inst != nullptr);
        }

        void VisitSub(target_code::Sub* inst) const override {
            assert(inst != nullptr);
        }

        void VisitMul(target_code::Mul* inst) const override {
            assert(inst != nullptr);
        }

        void VisitDiv(target_code::Div* inst) const override {
            assert(inst != nullptr);
        }

        void VisitMod(target_code::Mod* inst) const override {
            assert(inst != nullptr);
        }

        void VisitJeq(target_code::Jeq* inst) const override {
            assert(inst != nullptr);
        }

        void VisitJne(target_code::Jne* inst) const override {
            assert(inst != nullptr);
        }

        void VisitJgt(target_code::Jgt* inst) const override {
            assert(inst != nullptr);
        }

        void VisitJlt(target_code::Jlt* inst) const override {
            assert(inst != nullptr);
        }

        void VisitJge(target_code::Jge* inst) const override {
            assert(inst != nullptr);
        }

        void VisitJle(target_code::Jle* inst) const override {
            assert(inst != nullptr);
        }

        void VisitJump(target_code::Jump* inst) const override {
            assert(inst != nullptr);
        }

        void VisitCallFunc(target_code::CallFunc* inst) const override {
            assert(inst != nullptr);
            auto func =
                cpu::translate_operand(inst->get_result(), registers::ax);
            call_save_environment();    
            call_memcell(func);
        }

        void VisitPushArg(target_code::PushArg* inst) const override {
            assert(inst != nullptr);
        }

        void VisitEnterFunc(target_code::EnterFunc* inst) const override {
            assert(inst != nullptr);
        }

        void VisitExitFunc(target_code::ExitFunc* inst) const override {
            assert(inst != nullptr);
        }

        void VisitNewTable(target_code::NewTable* inst) const override {
            assert(inst != nullptr);
        }

        void VisitTableGetElem(target_code::TableGetElem* inst) const override {
            assert(inst != nullptr);
        }

        void VisitTableSetElem(target_code::TableSetElem* inst) const override {
            assert(inst != nullptr);
        }
    };
    }

    namespace cpu
    {

    void  execute(target_code::Instruction* instr) {
        PRECONDITION(instr != nullptr);
        InstructionExecuter* executer = new InstructionExecuter();
        instr->Accept(executer);
    }

    }
}