#ifndef VM_ARG_H
#define VM_ARG_H

#include "intermediate_code/syntax_analysis/expression/expression.h"
#include <ostream>

namespace target_code {
    /**
     * @brief A virtual machine agument type.
     * 
     */
    enum Vmarg_t {
        GLOBAL_a,
        LOCAL_a,
        FORMAL_a,
        BOOL_a,
        STRING_a,
        NUMBER_a,
        NIL_a,
        LIBFUNC_a,
        USERFUNC_a,
        LABEL_a,
        RETVAL_a
    };
    /**
     * @brief A virtual machine argument.
     * 
     */
    typedef struct vmarg {
        /**
         * @brief The type of this virtual machine argument.
         * 
         */
        Vmarg_t         type;
        /**
         * @brief The mapped value of this virtual machine argument.
         * 
         */
        unsigned int    value;
        /**
         * @brief Constructs a new Vmarg object.
         * 
         */
        vmarg() = default;
        vmarg(Vmarg_t, unsigned int value) : type(type), value(value) {};
        vmarg(Vmarg_t type) : type(type) {};
        vmarg(unsigned int value) : value(value) {};

        friend std::ostream &operator<<(std::ostream &os, const vmarg &rhs);
    } Vmarg;
}

#endif 