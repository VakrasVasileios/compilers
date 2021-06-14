#include "target_code.h"

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
    IopCodeDispatcher::generate_ASSIGN(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::ASSIGN_t);
        quad->taddress = NextInstructionLabel();
        Emit(new Assign(NextInstructionLabel(), make_operand(quad->result),
            make_operand(quad->arg1), quad->line));
    }

    void    
    IopCodeDispatcher::generate_ADD(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::ADD_t);
        quad->taddress = NextInstructionLabel();
        Emit(new Add(NextInstructionLabel(), make_operand(quad->result),
            make_operand(quad->arg1),make_operand(quad->arg2), quad->line));
    }

    void
    IopCodeDispatcher::generate_SUB(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::SUB_t);
        quad->taddress = NextInstructionLabel();
        Emit(new Sub(NextInstructionLabel(), make_operand(quad->result),
            make_operand(quad->arg1),make_operand(quad->arg2), quad->line));
    }

    void
    IopCodeDispatcher::generate_MUL(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::MUL_t);
        quad->taddress = NextInstructionLabel();
        Emit(new Mul(NextInstructionLabel(), make_operand(quad->result),
            make_operand(quad->arg1),make_operand(quad->arg2), quad->line));
    }

    void
    IopCodeDispatcher::generate_DIV(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::DIV_t);
        quad->taddress = NextInstructionLabel();
        Emit(new Div(NextInstructionLabel(), make_operand(quad->result),
            make_operand(quad->arg1),make_operand(quad->arg2), quad->line));
    }

    void
    IopCodeDispatcher::generate_MOD(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::MOD_t);
        quad->taddress = NextInstructionLabel();
        Emit(new Mod(NextInstructionLabel(), make_operand(quad->result),
            make_operand(quad->arg1),make_operand(quad->arg2), quad->line));
    }

    void
    IopCodeDispatcher::generate_UMINUS(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::UMINUS_t);
        quad->taddress = NextInstructionLabel();
        Emit(new Mul(NextInstructionLabel(), make_operand(quad->result),
        make_operand(quad->arg1), make_operand(new expression::IntConstant(-1)),
        quad->line ));
    }

    void
    IopCodeDispatcher::generate_JUMP(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::JUMP_t);
        Jump* inst;
        auto i_target_address = quad->result->to_unsigned();
        if (i_target_address < quad->label) {
            inst = new Jump(NextInstructionLabel(), new LabelVmarg(
            intermediate_code::quads[i_target_address - 1]->taddress),
            quad->line);
        } else {
            inst = new Jump(NextInstructionLabel(), new LabelVmarg(),
            quad->line);
            AddIncompleteJump(NextInstructionLabel(), i_target_address);
        }
        quad->taddress = NextInstructionLabel();
        Emit(inst);    
    }

    void
    IopCodeDispatcher::generate_IF_EQ(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_EQ_t);
        Jeq* inst;
        auto i_target_address = quad->arg2->to_unsigned();
        if (i_target_address < quad->label) {
            inst = new Jeq(NextInstructionLabel(), new LabelVmarg(
            intermediate_code::quads[i_target_address - 1]->taddress),
            make_operand(quad->result), make_operand(quad->arg1), quad->line);
        } else {
            inst = new Jeq(NextInstructionLabel(), new LabelVmarg(),
            make_operand(quad->result), make_operand(quad->arg1), quad->line);
            AddIncompleteJump(NextInstructionLabel(), i_target_address);
        }
        quad->taddress = NextInstructionLabel();
        Emit(inst);
    }

    void
    IopCodeDispatcher::generate_IF_NOTEQ(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_NOTEQ_t);
        Jne* inst;
        auto i_target_address = quad->arg2->to_unsigned();
        if (i_target_address < quad->label) {
            inst = new Jne(NextInstructionLabel(), new LabelVmarg(
            intermediate_code::quads[i_target_address - 1]->taddress),
            make_operand(quad->result), make_operand(quad->arg1), quad->line);
        } else {
            inst = new Jne(NextInstructionLabel(), new LabelVmarg(),
            make_operand(quad->result), make_operand(quad->arg1), quad->line);
            AddIncompleteJump(NextInstructionLabel(), i_target_address);
        }
        quad->taddress = NextInstructionLabel();
        Emit(inst);
    }

    void    
    IopCodeDispatcher::generate_IF_LESSEQ(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_LESSEQ_t);
        Jle* inst;
        auto i_target_address = quad->arg2->to_unsigned();
        if (i_target_address < quad->label) {
            inst = new Jle(NextInstructionLabel(), new LabelVmarg(
            intermediate_code::quads[i_target_address - 1]->taddress),
            make_operand(quad->result), make_operand(quad->arg1), quad->line);
        } else {
            inst = new Jle(NextInstructionLabel(), new LabelVmarg(),
            make_operand(quad->result), make_operand(quad->arg1), quad->line);
            AddIncompleteJump(NextInstructionLabel(), i_target_address);
        }
        quad->taddress = NextInstructionLabel();
        Emit(inst);
    }

    void
    IopCodeDispatcher::generate_IF_GREATEREQ(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_GREATEREQ_t);
        Jge* inst;
        auto i_target_address = quad->arg2->to_unsigned();
        if (i_target_address < quad->label) {
            inst = new Jge(NextInstructionLabel(), new LabelVmarg(
            intermediate_code::quads[i_target_address - 1]->taddress),
            make_operand(quad->result), make_operand(quad->arg1), quad->line);
        } else {
            inst = new Jge(NextInstructionLabel(), new LabelVmarg(),
            make_operand(quad->result), make_operand(quad->arg1), quad->line);
            AddIncompleteJump(NextInstructionLabel(), i_target_address);
        }
        quad->taddress = NextInstructionLabel();
        Emit(inst);
    }

    void
    IopCodeDispatcher::generate_IF_LESS(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_LESS_t);
        Jlt* inst;
        auto i_target_address = quad->arg2->to_unsigned();
        if (i_target_address < quad->label) {
            inst = new Jlt(NextInstructionLabel(), new LabelVmarg(
            intermediate_code::quads[i_target_address - 1]->taddress),
            make_operand(quad->result), make_operand(quad->arg1), quad->line);
        } else {
            inst = new Jlt(NextInstructionLabel(), new LabelVmarg(),
            make_operand(quad->result), make_operand(quad->arg1), quad->line);
            AddIncompleteJump(NextInstructionLabel(), i_target_address);
        }
        quad->taddress = NextInstructionLabel();
        Emit(inst);
    }

    void    
    IopCodeDispatcher::generate_IF_GREATER(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_GREATER_t);
        Jgt* inst;
        auto i_target_address = quad->arg2->to_unsigned();
        if (i_target_address < quad->label) {
            inst = new Jgt(NextInstructionLabel(), new LabelVmarg(
            intermediate_code::quads[i_target_address - 1]->taddress),
            make_operand(quad->result), make_operand(quad->arg1), quad->line);
        } else {
            inst = new Jgt(NextInstructionLabel(), new LabelVmarg(),
            make_operand(quad->result), make_operand(quad->arg1), quad->line);
            AddIncompleteJump(NextInstructionLabel(), i_target_address);
        }
        quad->taddress = NextInstructionLabel();
        Emit(inst);
    }

    void    
    IopCodeDispatcher::generate_CALL(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::CALL_t);
        quad->taddress = NextInstructionLabel();
        Emit(new CallFunc(NextInstructionLabel(),
            make_operand(quad->result), quad->line));
    }

    void
    IopCodeDispatcher::generate_PARAM(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::PARAM_t);
        quad->taddress = NextInstructionLabel();
        Emit(new PushArg(NextInstructionLabel(),
            make_operand(quad->result), quad->line));
    }

    void    
    IopCodeDispatcher::generate_RETURN(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::RET_t);
        quad->taddress = NextInstructionLabel();    
        Emit(new Assign(NextInstructionLabel(),
            new RetValVmarg(RETVAL_VALUE), make_operand(quad->result),
            quad->line));
    }

    void
    IopCodeDispatcher::generate_GETRETVAL(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::GETRETVAL_t);
        quad->taddress = NextInstructionLabel();
        Emit(new Assign(NextInstructionLabel(), make_operand(quad->result),
            new RetValVmarg(RETVAL_VALUE), quad->line));
    }

    void
    IopCodeDispatcher::generate_FUNCSTART(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::FUNCSTART_t);
        quad->taddress = NextInstructionLabel();
        Emit(new EnterFunc(NextInstructionLabel(), 
            make_operand(quad->result), quad->line));
    }

    void    
    IopCodeDispatcher::generate_FUNCEND(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::FUNCEND_t);
        quad->taddress = NextInstructionLabel();
        Emit(new ExitFunc(NextInstructionLabel(), make_operand(quad->result),
            quad->line));
    }

    void    
    IopCodeDispatcher::generate_TABLECREATE(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::TABLECREATE_t);
        quad->taddress = NextInstructionLabel();
        Emit(new NewTable(NextInstructionLabel(), make_operand(quad->result),
            quad->line));
    }

    void    
    IopCodeDispatcher::generate_TABLEGETELEM(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::TABLEGETELEM_t);
        quad->taddress = NextInstructionLabel();
        Emit(new TableGetElem(NextInstructionLabel(), make_operand(quad->result)
            ,make_operand(quad->arg1), make_operand(quad->arg2), quad->line));
    }

    void    
    IopCodeDispatcher::generate_TABLESETELEM(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::TABLESETELEM_t);
        quad->taddress = NextInstructionLabel();
        Emit(new TableSetElem(NextInstructionLabel(), make_operand(quad->result)
            ,make_operand(quad->arg1), make_operand(quad->arg2), quad->line));
    }
}