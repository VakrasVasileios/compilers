#include "execute_instruction.h"
#include "translate.h"
#include "../../exec/include/execute_assign.h"
#include "../../exec/include/execute_arithmetic.h"
#include "../../exec/include/execute_function.h"
#include "../../exec/include/execute_table.h"
#include "../../exec/include/execute_branch.h"

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
                auto lv = translate_operand(inst->get_result(), nullptr);
                auto rv = translate_operand(inst->get_arg1(), registers::ax);
                exec::execute_assign(lv, rv);
            }

            void VisitAdd(target_code::Add* inst) const override {
                assert(inst != nullptr);
                registers::ax = translate_operand(inst->get_result(), nullptr);
                registers::bx = translate_operand(inst->get_arg1(),
                    registers::bx);
                registers::cx = translate_operand(inst->get_arg2(),
                    registers::cx);
                exec::execute_add(registers::ax , registers::bx, registers::cx);
            }

            void VisitSub(target_code::Sub* inst) const override {
                assert(inst != nullptr);
                registers::ax = translate_operand(inst->get_result(), nullptr);
                registers::bx = translate_operand(inst->get_arg1(),
                    registers::bx);
                registers::cx = translate_operand(inst->get_arg2(),
                    registers::cx);
                exec::execute_sub(registers::ax , registers::bx, registers::cx);
            }

            void VisitMul(target_code::Mul* inst) const override {
                assert(inst != nullptr);
                registers::ax = translate_operand(inst->get_result(), nullptr);
                registers::bx = translate_operand(inst->get_arg1(),
                    registers::bx);
                registers::cx = translate_operand(inst->get_arg2(),
                    registers::cx);
                exec::execute_mul(registers::ax , registers::bx, registers::cx);
            }

            void VisitDiv(target_code::Div* inst) const override {
                assert(inst != nullptr);
                registers::ax = translate_operand(inst->get_result(), nullptr);
                registers::bx = translate_operand(inst->get_arg1(),
                    registers::bx);
                registers::cx = translate_operand(inst->get_arg2(),
                    registers::cx);
                exec::execute_div(registers::ax , registers::bx, registers::cx);
            }

            void VisitMod(target_code::Mod* inst) const override {
                assert(inst != nullptr);
                registers::ax = translate_operand(inst->get_result(), nullptr);
                registers::bx = translate_operand(inst->get_arg1(),
                    registers::bx);
                registers::cx = translate_operand(inst->get_arg2(),
                    registers::cx);
                exec::execute_mod(registers::ax , registers::bx, registers::cx);
            }

            void VisitJeq(target_code::Jeq* inst) const override {
                assert(inst != nullptr);
                registers::ax = translate_operand(inst->get_result(), nullptr);
                registers::bx = translate_operand(inst->get_arg1(), registers::bx);
                registers::cx = translate_operand(inst->get_arg2(), registers::cx);
                exec::execute_jeq(registers::ax, registers::bx, registers::cx);
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
                exec::execute_callfunc(func);
            }

            void VisitPushArg(target_code::PushArg* inst) const override {
                assert(inst != nullptr);
                auto arg = translate_operand(inst->get_result(), registers::ax);
                exec::execute_pusharg(arg);
            }

            void VisitEnterFunc(target_code::EnterFunc* inst) const override {
                assert(inst != nullptr);
                auto func = translate_operand(inst->get_result(),
                    registers::ax);
                exec::execute_enterfunc(func);
            }

            void VisitExitFunc(target_code::ExitFunc* inst) const override {
                assert(inst != nullptr);
                exec::execute_exitfunc();
            }

            void VisitNewTable(target_code::NewTable* inst) const override {
                assert(inst != nullptr);
                auto lv = translate_operand(inst->get_result(), nullptr);
                exec::execute_newtable(lv);
            }

            void VisitTableGetElem(target_code::TableGetElem* inst) const
            override {
                assert(inst != nullptr);
                auto lv = translate_operand(inst->get_result(), nullptr);
                auto table = translate_operand(inst->get_arg1(), nullptr);
                auto key = translate_operand(inst->get_arg2(), registers::ax);
                exec::execute_tablegetelem(lv, table, key);
            }

            void VisitTableSetElem(target_code::TableSetElem* inst) const
            override {
                assert(inst != nullptr);
                auto table = translate_operand(inst->get_result(), nullptr);
                auto key = translate_operand(inst->get_arg1(), registers::ax);
                auto value = translate_operand(inst->get_arg1(), registers::bx);
                exec::execute_tablesetelem(table, key, value);
            }
        };
    }

    void  execute_instruction(target_code::Instruction* instr) {
        PRECONDITION(instr != nullptr);
        InstructionExecuter* executer = new InstructionExecuter();
        instr->Accept(executer);
    }

    }
}