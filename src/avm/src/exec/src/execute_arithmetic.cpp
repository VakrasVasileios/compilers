#include "../include/execute_arithmetic.h"
#include "../include/execute_assign.h"

namespace avm
{
    namespace exec
    {
        void execute_add(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
        memcell::AvmMemcell* arg2) {
            PRECONDITION(res != nullptr);
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            execute_assign(res, *arg1 + *arg2);
        }

        void execute_sub(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
        memcell::AvmMemcell* arg2) {
            PRECONDITION(res != nullptr);
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            execute_assign(res, *arg1 - *arg2);
        }

        void execute_mul(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
        memcell::AvmMemcell* arg2) {
            PRECONDITION(res != nullptr);
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            execute_assign(res, *arg1 * *arg2);
        }

        void execute_div(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
        memcell::AvmMemcell* arg2) {
            PRECONDITION(res != nullptr);
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            execute_assign(res, *arg1 / *arg2);
        }

        void execute_mod(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
        memcell::AvmMemcell* arg2) {
            PRECONDITION(res != nullptr);
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            execute_assign(res, *arg1 % *arg2);
        }
    }
}