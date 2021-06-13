#ifndef INCOMPLETE_JUMP_H
#define INCOMPLETE_JUMP_H

namespace target_code {
    /**
     * @brief An unpatched  jump virtual machine
     * argument.
     * 
     */
    typedef struct incomplete_jump {
        /**
         * @brief The label of the instruction holding 
         * this incomplete_jump.
         * 
         */
        unsigned int label;
        /**
         * @brief The i-code jump target address of this
         * incomplete_jump.
         * 
         */
        unsigned int i_target_address;
        /**
         * @brief Constructs a new incomplete jump object
         * 
         * @param label the label of the instruction holding 
         * the new incomplete_jump.
         * @param i_target_address the i-code jump target address
         * of the new incomplete_jump.
         */
        incomplete_jump(unsigned int label, unsigned int i_target_address) : label(label), i_target_address(i_target_address) {};
    } IncompleteJump;
}

#endif