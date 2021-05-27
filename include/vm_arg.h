#ifndef VM_ARG_H
#define VM_ARG_H

/**
 * @brief A virtual machine agument type.
 * 
 */
enum Vmarg_t {
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

#endif 
