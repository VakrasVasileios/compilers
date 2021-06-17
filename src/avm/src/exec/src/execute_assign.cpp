#include "../include/execute_assign.h"

namespace avm
{
    namespace exec
    {
    void execute_assign(memcell::AvmMemcell* lv, memcell::AvmMemcell* rv) {
        PRECONDITION(lv != nullptr);
        PRECONDITION(rv != nullptr);
        delete lv;
        lv = rv;
        POSTCONDITION(lv != nullptr);
    }
    }
}