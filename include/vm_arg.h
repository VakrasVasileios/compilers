#ifndef VM_ARG_H
#define VM_ARG_H

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
    USERFUNC_a
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
} Vmarg;


void make_operand (Expression* expr, Vmarg* arg);

#endif 
