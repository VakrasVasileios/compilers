#include "../include/execute_assign.h"

namespace avm
{
    namespace exec
    {
        namespace 
        {
            void assign_iftablerv(memcell::AvmMemcell** lv,
                const memcell::AvmMemcell* rv) {
                assert(lv != nullptr);
                assert(rv != nullptr);
                
                if (auto trv = dynamic_cast<const memcell::TableMemcell*>(rv)) {
                    auto tlv = new memcell::TableMemcell();
                    delete *lv;
                    tlv = trv->clone();
                    *lv = tlv;
                } else {
                    delete *lv;
                    *lv = rv->clone();
                }
            }
        }
    void execute_assign(memcell::AvmMemcell** lv, 
        const memcell::AvmMemcell* rv) {
        PRECONDITION(lv != nullptr);
        PRECONDITION(rv != nullptr);
        assign_iftablerv(lv, rv);
        POSTCONDITION(lv != nullptr);
    }
    }
}