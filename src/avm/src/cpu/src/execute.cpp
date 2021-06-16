#include "execute.h"
#include "translate.h"
#include "execute_function.h"

#define AVM_NUMACTUALS_OFFSET   +4
#define AVM_SAVEDPC_OFFSET      +3
#define AVM_SAVEDTOP_OFFSET     +2
#define AVM_SAVEDTOPSP_OFFSET   +1

namespace avm
{
    namespace cpu
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
                registers::ax = translate_operand(inst->get_result(), nullptr);
                registers::bx = translate_operand(inst->get_arg1(), registers::bx);
                registers::cx = translate_operand(inst->get_arg2(), registers::cx);

                delete registers::ax;
                registers::ax = *registers::bx + registers::cx;
            }

            void VisitSub(target_code::Sub* inst) const override {
                assert(inst != nullptr);
                registers::ax = translate_operand(inst->get_result(), nullptr);
                registers::bx = translate_operand(inst->get_arg1(), registers::bx);
                registers::cx = translate_operand(inst->get_arg2(), registers::cx);

                delete registers::ax;
                registers::ax = *registers::bx - registers::cx;
            }

            void VisitMul(target_code::Mul* inst) const override {
                assert(inst != nullptr);
                registers::ax = translate_operand(inst->get_result(), nullptr);
                registers::bx = translate_operand(inst->get_arg1(), registers::bx);
                registers::cx = translate_operand(inst->get_arg2(), registers::cx);

                delete registers::ax;
                registers::ax = *registers::bx * registers::cx;
            }

            void VisitDiv(target_code::Div* inst) const override {
                assert(inst != nullptr);
                registers::ax = translate_operand(inst->get_result(), nullptr);
                registers::bx = translate_operand(inst->get_arg1(), registers::bx);
                registers::cx = translate_operand(inst->get_arg2(), registers::cx);

                delete registers::ax;
                registers::ax = *registers::bx / registers::cx;
            }

            void VisitMod(target_code::Mod* inst) const override {
                assert(inst != nullptr);
                registers::ax = translate_operand(inst->get_result(), nullptr);
                registers::bx = translate_operand(inst->get_arg1(), registers::bx);
                registers::cx = translate_operand(inst->get_arg2(), registers::cx);

                delete registers::ax;
                registers::ax = *registers::bx % registers::cx;
            }

            void VisitJeq(target_code::Jeq* inst) const override {
                assert(inst != nullptr);
                registers::ax = translate_operand(inst->get_result(), nullptr);
                registers::bx = translate_operand(inst->get_arg1(), registers::bx);
                registers::cx = translate_operand(inst->get_arg2(), registers::cx);

                if (registers::bx == registers::cx) {
                    assert(dynamic_cast<memcell::NumMemcell*>(registers::ax) != nullptr);
                    registers::pc = dynamic_cast<memcell::NumMemcell*>(registers::ax)->num_val();
                }
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
                auto func = translate_operand(inst->get_result(),
                    registers::ax);
                    
                save_environment();    
                call_memcell(func);
            }

            void VisitPushArg(target_code::PushArg* inst) const override {
                assert(inst != nullptr);
                auto arg = translate_operand(inst->get_result(), registers::ax);
                memory::stack_segment.push(arg);
                ++total_actuals;
            }

            void VisitEnterFunc(target_code::EnterFunc* inst) const override {
                assert(inst != nullptr);
                auto func = translate_operand(inst->get_result(),
                    registers::ax);

                total_actuals = 0;
                auto func_info = memory::Constants::GetInstance().GetUserfunc(
                    registers::pc);
                registers::topsp = registers::top;
                registers::top = registers::top - func_info.local_count;
            }

            void VisitExitFunc(target_code::ExitFunc* inst) const override {
                assert(inst != nullptr);
                auto old_top = registers::top;
                registers::top = get_envvalue(registers::topsp +
                    AVM_SAVEDTOP_OFFSET);    
                registers::pc = get_envvalue(registers::topsp +
                    AVM_SAVEDPC_OFFSET);  
                registers::topsp = get_envvalue(registers::topsp +
                    AVM_SAVEDTOPSP_OFFSET); 

                while(++old_top <= registers::top)
                    memory::stack_segment.pop();       
            }

            void VisitNewTable(target_code::NewTable* inst) const override {
                assert(inst != nullptr);
            }

            void VisitTableGetElem(target_code::TableGetElem* inst) const
            override {
                assert(inst != nullptr);
            }

            void VisitTableSetElem(target_code::TableSetElem* inst) const
            override {
                assert(inst != nullptr);
            }
        };
        }

    void  execute(target_code::Instruction* instr) {
        PRECONDITION(instr != nullptr);
        InstructionExecuter* executer = new InstructionExecuter();
        instr->Accept(executer);
    }

    }
}