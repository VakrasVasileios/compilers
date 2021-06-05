#include "../../include/target_code/target_code.h"

#define RETVAL_VALUE 69420

namespace target_code {
    Vmarg* 
    IopCodeDispatcher::make_operand (expression::Expression* expr) {
        if(expr != nullptr) {
            expr->Accept(make_op_visitor);

            return make_op_visitor->get_result();
        }
        return nullptr;
    }
    /* 
    The dispatch takes place because the generate functions index in this
    array matches each respective opcode enum value, from which each  vm
    instruction is generated.                               
    */
    void    
    IopCodeDispatcher::Generate(){
        for (auto quad : intermediate_code::quads)
            (this->*generators[quad->op])(quad);
        PatchIncompleteJumps();    
    }

    void        
    IopCodeDispatcher::generate(Vmopcode op,  intermediate_code::Quad* quad) {
        quad->taddress = NextInstructionLabel();
        Emit(new Instruction(NextInstructionLabel(), op, make_operand(quad->result),
            make_operand(quad->arg1), make_operand(quad->arg2), quad->line));
    }

    void 
    IopCodeDispatcher::generate_branch(Vmopcode op, intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(intermediate_code::IsBranchQuad(quad));
        auto inst = new Instruction(NextInstructionLabel(), op, new Vmarg(),
                        make_operand(quad->result), make_operand(quad->arg1), quad->line);

        inst->result->type = LABEL_a;
        auto i_target_address = quad->arg2->to_unsigned();
        if (i_target_address < quad->label)
            inst->result->value = intermediate_code::quads[i_target_address - 1]->taddress;
        else
            AddIncompleteJump(NextInstructionLabel(), i_target_address);
        quad->taddress = NextInstructionLabel();
        Emit(inst);    
    }

    void    
    IopCodeDispatcher::generate_ASSIGN(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::ASSIGN_t);
        quad->taddress = NextInstructionLabel();
        Emit(new Instruction(NextInstructionLabel(), ASSIGN_VM, make_operand(quad->result),
            make_operand(quad->arg1), nullptr, quad->line));
    }

    void    
    IopCodeDispatcher::generate_ADD(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::ADD_t);
        generate(ADD_VM, quad);
    }

    void
    IopCodeDispatcher::generate_SUB(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::SUB_t);
        generate(SUB_VM, quad);
    }

    void
    IopCodeDispatcher::generate_MUL(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::MUL_t);
        generate(MUL_VM, quad);
    }

    void
    IopCodeDispatcher::generate_DIV(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::DIV_t);
        generate(DIV_VM, quad);
    }

    void
    IopCodeDispatcher::generate_MOD(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::MOD_t);
        generate(MOD_VM, quad);
    }

    void
    IopCodeDispatcher::generate_UMINUS(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::UMINUS_t);
        quad->taddress = NextInstructionLabel();
        Emit(new Instruction(NextInstructionLabel(), MUL_VM, make_operand(quad->result),
            make_operand(quad->arg1), make_operand(new expression::IntConstant(-1)), quad->line));
    }

    void
    IopCodeDispatcher::generate_JUMP(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::JUMP_t);
        auto inst = new Instruction(NextInstructionLabel(), JUMP_VM, new Vmarg(), nullptr, nullptr, quad->line);

        inst->result->type = LABEL_a;
        auto i_target_address = quad->result->to_unsigned();
        if (i_target_address < quad->label)
            inst->result->value = intermediate_code::quads[i_target_address - 1]->taddress;
        else
            AddIncompleteJump(NextInstructionLabel(), i_target_address);
        quad->taddress = NextInstructionLabel();
        Emit(inst);    
    }

    void
    IopCodeDispatcher::generate_IF_EQ(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_EQ_t);
        generate_branch(JEQ_VM, quad);
    }

    void
    IopCodeDispatcher::generate_IF_NOTEQ(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_NOTEQ_t);
        generate_branch(JNE_VM, quad);
    }

    void    
    IopCodeDispatcher::generate_IF_LESSEQ(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_LESSEQ_t);
        generate_branch(JLE_VM, quad);
    }

    void
    IopCodeDispatcher::generate_IF_GREATEREQ(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_GREATEREQ_t);
        generate_branch(JGE_VM, quad);
    }

    void
    IopCodeDispatcher::generate_IF_LESS(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_LESS_t);
        generate_branch(JLT_VM, quad);
    }

    void    
    IopCodeDispatcher::generate_IF_GREATER(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_GREATER_t);
        generate_branch(JGT_VM, quad);
    }

    void    
    IopCodeDispatcher::generate_CALL(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::CALL_t);
        quad->taddress = NextInstructionLabel();
        Emit(new Instruction(NextInstructionLabel(),
             CALLFUNC_VM, make_operand(quad->result), nullptr, nullptr, quad->line));
    }

    void
    IopCodeDispatcher::generate_PARAM(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::PARAM_t);
        quad->taddress = NextInstructionLabel();
        Emit(new Instruction(NextInstructionLabel(),
            PUSHARG_VM, make_operand(quad->result), nullptr, nullptr, quad->line));
    }

    void    
    IopCodeDispatcher::generate_RETURN(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::RET_t);
        quad->taddress = NextInstructionLabel();    
        Emit(new Instruction(NextInstructionLabel(),
            ASSIGN_VM, new Vmarg(RETVAL_a, RETVAL_VALUE), make_operand(quad->result), nullptr, quad->line));
    }

    void
    IopCodeDispatcher::generate_GETRETVAL(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::GETRETVAL_t);
        quad->taddress = NextInstructionLabel();
        Emit(new Instruction(NextInstructionLabel(), ASSIGN_VM,
             make_operand(quad->result), new Vmarg(RETVAL_a, RETVAL_VALUE), nullptr, quad->line));
    }

    void
    IopCodeDispatcher::generate_FUNCSTART(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::FUNCSTART_t);
        quad->taddress = NextInstructionLabel();
        Emit(new Instruction(NextInstructionLabel(), ENTERFUNC_VM,
            make_operand(quad->result), nullptr, nullptr, quad->line));
    }

    void    
    IopCodeDispatcher::generate_FUNCEND(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::FUNCEND_t);
        quad->taddress = NextInstructionLabel();
        
        Emit(new Instruction(NextInstructionLabel(), EXITFUNC_VM,
            make_operand(quad->result), nullptr, nullptr, quad->line));
    }

    void    
    IopCodeDispatcher::generate_TABLECREATE(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::TABLECREATE_t);
        generate(NEWTABLE_VM, quad);
    }

    void    
    IopCodeDispatcher::generate_TABLEGETELEM(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::TABLEGETELEM_t);
        generate(TABLEGETELEM_VM, quad);
    }

    void    
    IopCodeDispatcher::generate_TABLESETELEM(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::TABLESETELEM_t);
        generate(TABLESETELEM_VM, quad);
    }
}