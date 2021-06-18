#ifndef EXECUTE_ASSIGN_H
#define EXECUTE_ASSIGN_H

#include "../../../../../util/contract/include/contract.h"
#include "../../memcell/include/avm_memcell.h"

namespace avm
{
    namespace exec
    {
    /**
     * @brief Executes an assign operation.
     * 
     * @param lv the lvalue memcell, not null
     * @param rv the rvalue memcell, not null
     */
    void execute_assign(memcell::AvmMemcell** lv,
        const memcell::AvmMemcell* rv);
    }
}

#endif