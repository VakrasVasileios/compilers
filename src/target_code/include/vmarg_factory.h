#ifndef VMARG_FACTORY_H
#define VMARG_FACTORY_H

#include "../../../util/contract/include/contract.h"
#include "vm_arg.h"

namespace target_code 
{
/**
 * @brief Creates a new vmarg instance based on a type and a value.
 * 
 * @param type the type to base the new vmarg instance
 * @param value the value to base the new vmarg instance, greater or equal
 * to zero
 * @return a read/write access to a new vmarg instance based on a type
 * and a value, not null
 */
Vmarg* create_vmarg(Vmarg_t type, unsigned int value);
}

#endif