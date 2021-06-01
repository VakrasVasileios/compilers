#ifndef TARGET_CODE_H
#define TARGET_CODE_H

#include <vector>
#include "intermediate_code/intermediate_code.h"
#include "instruction.h"
#include "vm_arg.h"
#include "program_consts.h"

/**
 * @brief Namespace for generating the target code.
 * 
 */
namespace target_code {
    /**
     * @brief A list of all the emitted target code
     * instructions.
     * 
     */
    std::vector<Instruction*>   instructions;
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
        void                    Generate();
    private:
        Vmarg*                  make_operand (expression::Expression* expr);
        Vmarg*                  make_numberoperand (expression::Expression* expr);
        Vmarg*                  make_booloperand (expression::Expression* expr);
        Vmarg*                  make_retvaloperand (expression::Expression* expr);

        void                    generate(Vmopcode op, intermediate_code::Quad* quad);
        void                    generate_relational(Vmopcode op, intermediate_code::Quad* quad);
        void                    generate_ASSIGN(intermediate_code::Quad* quad);
        void                    generate_ADD(intermediate_code::Quad* quad);
        void                    generate_SUB(intermediate_code::Quad* quad);
        void                    generate_MUL(intermediate_code::Quad* quad);
        void                    generate_DIV(intermediate_code::Quad* quad);
        void                    generate_MOD(intermediate_code::Quad* quad);
        void                    generate_UMINUS(intermediate_code::Quad* quad);
        void                    generate_JUMP(intermediate_code::Quad* quad);
        void                    generate_IF_EQ(intermediate_code::Quad* quad);
        void                    generate_IF_NOTEQ(intermediate_code::Quad* quad);
        void                    generate_IF_LESSEQ(intermediate_code::Quad* quad);
        void                    generate_IF_GREATEREQ(intermediate_code::Quad* quad);
        void                    generate_IF_LESS(intermediate_code::Quad* quad); 
        void                    generate_IF_GREATER(intermediate_code::Quad* quad);
        void                    generate_CALL(intermediate_code::Quad* quad);
        void                    generate_PARAM(intermediate_code::Quad* quad);
        void                    generate_RETURN(intermediate_code::Quad* quad);
        void                    generate_GETRETVAL(intermediate_code::Quad* quad);
        void                    generate_FUNCSTART(intermediate_code::Quad* quad);
        void                    generate_FUNCEND(intermediate_code::Quad* quad);
        void                    generate_TABLECREATE(intermediate_code::Quad* quad);
        void                    generate_TABLEGETELEM(intermediate_code::Quad* quad);
        void                    generate_TABLESETELEM(intermediate_code::Quad* quad);

        typedef void (IopCodeDispatcher:: *generator_func_t) (intermediate_code::Quad*);
        generator_func_t    generators[26];        
    };
    /**
     * @brief An opcode dispatcher, which generates target
     * code instructions by dispatching the iopcode of the
     * emitted quads during the intermediate code production.
     * 
     */
    IopCodeDispatcher           opcode_dispatcher;
    /**
     * @brief Emits a target code instruction.
     * 
     * @param emitted the target code instruction to
     * be emitted, not null
     */
    void                        Emit(Instruction* emitted);
    /**
     * @brief Returns a read/write access to the label
     * after the most recently emitted instuction.
     * 
     * @return a read/write access to the label
     * after the most recently emitted instuction, greater or equal to zero 
     */
    unsigned int                 NextInstructionLabel();
}

#endif