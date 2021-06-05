#ifndef VM_ARG_H
#define VM_ARG_H

#include "intermediate_code/syntax_analysis/expression/expression.h"
#include <ostream>

namespace target_code {
    /**
     * @brief A virtual machine agument type.
     * 
     */
    enum Vmarg_t : int {
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
        vmarg(Vmarg_t _type, unsigned int _value) : type(_type), value(_value) {};
        vmarg(Vmarg_t _type) : type(_type) {};
        vmarg(unsigned int _value) : value(_value) {};

        friend std::ostream &operator<<(std::ostream &os, const vmarg &rhs);
    } Vmarg;

    std::string type_to_string(Vmarg_t _type);
}

#endif 