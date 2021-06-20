#include "execute_instruction.h"
#include "translate.h"
#include "../../exec/include/execute_assign.h"
#include "../../exec/include/execute_arithmetic.h"
#include "../../exec/include/execute_function.h"
#include "../../exec/include/execute_table.h"
#include "../../exec/include/execute_branch.h"
#include <iostream>

namespace avm
{
    namespace cpu
    {
    namespace
    {
        namespace
        {
            unsigned get_label(target_code::Vmarg* label) {
                auto l = dynamic_cast<target_code::LabelVmarg*>(label);
                assert(l != nullptr);
                return l->get_value();
            }
        }

        class InstructionExecuter : public target_code::InstructionVisitor {
        public:
            InstructionExecuter() = default;
            ~InstructionExecuter() = default;

            void VisitAssign(target_code::Assign* inst) const override {
                assert(inst != nullptr);
                auto lv = translate_operand(inst->get_result(), &registers::ax);
                auto rv = *translate_operand(inst->get_arg1(), &registers::bx);
                // if (inst->get_result()->get_type() == target_code::RETVAL_a)
                //     exec::execute_assign(&registers::retval, rv);
                // else if (inst->get_arg1()->get_type() == target_code::RETVAL_a)
                //     exec::execute_assign(lv, registers::retval);
                // else
                exec::execute_assign(lv, rv);
            }

            void VisitAdd(target_code::Add* inst) const override {
                assert(inst != nullptr);
                auto lv = translate_operand(inst->get_result(), &registers::ax);
                auto rv1 = *translate_operand(inst->get_arg1(),
                    &registers::bx);
                auto rv2 = *translate_operand(inst->get_arg2(),
                    &registers::cx);
                exec::execute_add(lv, rv1, rv2);
            }

            void VisitSub(target_code::Sub* inst) const override {
                assert(inst != nullptr);
                auto lv = translate_operand(inst->get_result(), &registers::ax);
                auto rv1 = *translate_operand(inst->get_arg1(),
                    &registers::bx);
                auto rv2 = *translate_operand(inst->get_arg2(),
                    &registers::cx);
                exec::execute_sub(lv , rv1, rv2);
            }

            void VisitMul(target_code::Mul* inst) const override {
                assert(inst != nullptr);
                auto lv = translate_operand(inst->get_result(), &registers::ax);
                auto rv1 = *translate_operand(inst->get_arg1(),
                    &registers::bx);
                auto rv2 = *translate_operand(inst->get_arg2(),
                    &registers::cx);
                exec::execute_mul(lv, rv1, rv2);
            }

            void VisitDiv(target_code::Div* inst) const override {
                assert(inst != nullptr);
                auto lv = translate_operand(inst->get_result(), &registers::ax);
                auto rv1 = *translate_operand(inst->get_arg1(),
                    &registers::bx);
                auto rv2 = *translate_operand(inst->get_arg2(),
                    &registers::cx);
                exec::execute_div(lv, rv1, rv2);
            }

            void VisitMod(target_code::Mod* inst) const override {
                assert(inst != nullptr);
                auto lv = translate_operand(inst->get_result(), &registers::ax);
                auto rv1 = *translate_operand(inst->get_arg1(),
                    &registers::bx);
                auto rv2 = *translate_operand(inst->get_arg2(),
                    &registers::cx);
                exec::execute_mod(lv, rv1, rv2);
            }

            void VisitJeq(target_code::Jeq* inst) const override {
                assert(inst != nullptr);
                auto rv1 = *translate_operand(inst->get_arg1(), &registers::ax);
                auto rv2 = *translate_operand(inst->get_arg2(), &registers::bx);
                exec::execute_jeq(get_label(inst->get_result()), rv1, rv2);
            }

            void VisitJne(target_code::Jne* inst) const override {
                assert(inst != nullptr);
                auto rv1 = *translate_operand(inst->get_arg1(), &registers::ax);
                auto rv2 = *translate_operand(inst->get_arg2(), &registers::bx);
                exec::execute_jne(get_label(inst->get_result()), rv1, rv2);
            }

            void VisitJgt(target_code::Jgt* inst) const override {
                assert(inst != nullptr);
                auto rv1 = *translate_operand(inst->get_arg1(), &registers::ax);
                auto rv2 = *translate_operand(inst->get_arg2(), &registers::bx);
                exec::execute_jgt(get_label(inst->get_result()), rv1, rv2);
            }

            void VisitJlt(target_code::Jlt* inst) const override {
                assert(inst != nullptr);
                auto rv1 = *translate_operand(inst->get_arg1(), &registers::ax);
                auto rv2 = *translate_operand(inst->get_arg2(), &registers::bx);
                exec::execute_jlt(get_label(inst->get_result()), rv1, rv2);
            }

            void VisitJge(target_code::Jge* inst) const override {
                assert(inst != nullptr);
                auto rv1 = *translate_operand(inst->get_arg1(), &registers::ax);
                auto rv2 = *translate_operand(inst->get_arg2(), &registers::bx);
                exec::execute_jge(get_label(inst->get_result()), rv1, rv2);
            }

            void VisitJle(target_code::Jle* inst) const override {
                assert(inst != nullptr);
                auto rv1 = *translate_operand(inst->get_arg1(), &registers::ax);
                auto rv2 = *translate_operand(inst->get_arg2(), &registers::bx);
                exec::execute_jle(get_label(inst->get_result()), rv1, rv2);
            }

            void VisitJump(target_code::Jump* inst) const override {
                assert(inst != nullptr);
                exec::execute_jmp(get_label(inst->get_result()));
            }

            void VisitCallFunc(target_code::CallFunc* inst) const override {
                assert(inst != nullptr);
                auto func = *translate_operand(inst->get_result(), 
                    &registers::ax);
                exec::execute_callfunc(func);
            }

            void VisitPushArg(target_code::PushArg* inst) const override {
                assert(inst != nullptr);
                auto arg = translate_operand(inst->get_result(), &registers::ax);
                exec::execute_pusharg(arg);
            }

            void VisitEnterFunc(target_code::EnterFunc* inst) const override {
                assert(inst != nullptr);
                auto func = *translate_operand(inst->get_result(),
                    &registers::ax);
                exec::execute_enterfunc(func);
            }

            void VisitExitFunc(target_code::ExitFunc* inst) const override {
                assert(inst != nullptr);
                exec::execute_exitfunc();
            }

            void VisitNewTable(target_code::NewTable* inst) const override {
                assert(inst != nullptr);
                auto lv = translate_operand(inst->get_result(), &registers::ax);
                exec::execute_newtable(lv);
            }

            void VisitTableGetElem(target_code::TableGetElem* inst) const
            override {
                assert(inst != nullptr);
                auto lv = translate_operand(inst->get_result(), &registers::ax);
                auto table = *translate_operand(inst->get_arg1(), &registers::bx);
                auto item = *translate_operand(inst->get_arg2(), &registers::cx);
                exec::execute_tablegetelem(lv, table, item);
            }

            void VisitTableSetElem(target_code::TableSetElem* inst) const
            override {
                assert(inst != nullptr);
                auto table = translate_operand(inst->get_result(), &registers::ax);
                auto key = *translate_operand(inst->get_arg1(), &registers::bx);
                auto item = *translate_operand(inst->get_arg1(), &registers::cx);
                exec::execute_tablesetelem(table, key, item);
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