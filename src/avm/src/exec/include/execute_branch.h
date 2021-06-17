#ifndef EXECUTE_BRANCH_H
#define EXECUTE_BRANCH_H

#include "../../../../../util/contract/include/contract.h"
#include "../../memcell/include/avm_memcell.h"

namespace avm
{
    namespace exec
    {
        void execute_jeq(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
            memcell::AvmMemcell* arg2);

        void execute_jne(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
            memcell::AvmMemcell* arg2);

        void execute_jgt(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
            memcell::AvmMemcell* arg2);

        void execute_jge(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
            memcell::AvmMemcell* arg2);

        void execute_jlt(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
            memcell::AvmMemcell* arg2);

        void execute_jle(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
            memcell::AvmMemcell* arg2);

        void execute_jmp(memcell::AvmMemcell* res);
    }
}

#endif