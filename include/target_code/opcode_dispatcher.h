#ifndef OPCODE_DISPATCHER_H
#define OPCODE_DISPATCHER_H

#include <assert.h>
#include <string>
#include "quad.h"
#include "../util/contract/contract.h"
#include "target_code.h"
#include "../include/virtual_machine/instruction.h"
#include "../include/virtual_machine/vm_arg.h"
#include "../include/expression/symbol.h"
#include "../include/expression/bool_constant.h"
#include "../include/expression/nil_constant.h"
#include "../include/expression/int_constant.h"
#include "../include/expression/double_constant.h"
#include "../include/expression/string_constant.h"
#include "../include/virtual_machine/program_consts.h"

namespace target_code {
    /**
     * @brief An opcode dispatcher.
     * 
     */
    class IopCodeDispatcher {
    public:
        /**
         * @brief Constructs a new IopCodeDispatcher object.
         * 
         */
        IopCodeDispatcher()
        : generators {
            &IopCodeDispatcher::generate_ASSIGN,
            &IopCodeDispatcher::generate_ADD,
            &IopCodeDispatcher::generate_SUB,
            &IopCodeDispatcher::generate_MUL,
            &IopCodeDispatcher::generate_DIV,
            &IopCodeDispatcher::generate_MOD,
            &IopCodeDispatcher::generate_UMINUS,
            &IopCodeDispatcher::generate_JUMP,
            &IopCodeDispatcher::generate_IF_EQ,
            &IopCodeDispatcher::generate_IF_NOTEQ,
            &IopCodeDispatcher::generate_IF_LESSEQ,
            &IopCodeDispatcher::generate_IF_GREATEREQ,
            &IopCodeDispatcher::generate_IF_LESS,
            &IopCodeDispatcher::generate_IF_GREATER,
            &IopCodeDispatcher::generate_CALL,
            &IopCodeDispatcher::generate_PARAM,
            &IopCodeDispatcher::generate_RETURN,
            &IopCodeDispatcher::generate_GETRETVAL,
            &IopCodeDispatcher::generate_FUNCSTART,
            &IopCodeDispatcher::generate_FUNCEND,
            &IopCodeDispatcher::generate_TABLECREATE,
            &IopCodeDispatcher::generate_TABLEGETELEM,
            &IopCodeDispatcher::generate_TABLESETELEM
        } {}
        /**
         * @brief Destroys this OpCodeDispatcher object.
         * 
         */
        ~IopCodeDispatcher() = default;
        /**
         * @brief Emits all of the target code instructions
         * from the list of intermediate code quads.  
         * 
         */
        void        Generate();
    private:
        virtual_machine::Vmarg*     make_operand (expression::Expression* expr);
        virtual_machine::Vmarg*     make_numberoperand (expression::Expression* expr);
        virtual_machine::Vmarg*     make_booloperand (expression::Expression* expr);
        virtual_machine::Vmarg*     make_retvaloperand (expression::Expression* expr);

        void                        generate(virtual_machine::Vmopcode op, Quad* quad);
        void                        generate_relational(virtual_machine::Vmopcode op, Quad* quad);
        void                        generate_ASSIGN(Quad* quad);
        void                        generate_ADD(Quad* quad);
        void                        generate_SUB(Quad* quad);
        void                        generate_MUL(Quad* quad);
        void                        generate_DIV(Quad* quad);
        void                        generate_MOD(Quad* quad);
        void                        generate_UMINUS(Quad* quad);
        void                        generate_JUMP(Quad* quad);
        void                        generate_IF_EQ(Quad* quad);
        void                        generate_IF_NOTEQ(Quad* quad);
        void                        generate_IF_LESSEQ(Quad* quad);
        void                        generate_IF_GREATEREQ(Quad* quad);
        void                        generate_IF_LESS(Quad* quad); 
        void                        generate_IF_GREATER(Quad* quad);
        void                        generate_CALL(Quad* quad);
        void                        generate_PARAM(Quad* quad);
        void                        generate_RETURN(Quad* quad);
        void                        generate_GETRETVAL(Quad* quad);
        void                        generate_FUNCSTART(Quad* quad);
        void                        generate_FUNCEND(Quad* quad);
        void                        generate_TABLECREATE(Quad* quad);
        void                        generate_TABLEGETELEM(Quad* quad);
        void                        generate_TABLESETELEM(Quad* quad);

        typedef void (IopCodeDispatcher:: *generator_func_t) (Quad*);
        generator_func_t    generators[26];        
    };
}

#endif
