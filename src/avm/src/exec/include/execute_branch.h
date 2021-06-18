#ifndef EXECUTE_BRANCH_H
#define EXECUTE_BRANCH_H

#include "../../../../../util/contract/include/contract.h"
#include "../../memcell/include/avm_memcell.h"

namespace avm
{
    namespace exec
    {
        void execute_jeq(const memcell::AvmMemcell* res,
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2);

        void execute_jne(const memcell::AvmMemcell* res,
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2);

        void execute_jgt(const memcell::AvmMemcell* res, 
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2);

        void execute_jge(const memcell::AvmMemcell* res,
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2);

        void execute_jlt(const memcell::AvmMemcell* res,
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2);

        void execute_jle(const memcell::AvmMemcell* res,
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2);

        void execute_jmp(const memcell::AvmMemcell* res);
    }
}

#endif