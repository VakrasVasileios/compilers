#include "execute_function.h"
#include "../../memory/include/memory.h"
#include "../../registers/include/registers.h"

namespace avm
{
    namespace cpu
    {

    unsigned total_actuals = 0;

    void call_save_environment() {
        memory::stack_segment.push(new memcell::NumMemcell(total_actuals));
        memory::stack_segment.push(new memcell::NumMemcell(registers::pc + 1));
        memory::stack_segment.push(new memcell::NumMemcell(
            registers::top + total_actuals + 1));
        memory::stack_segment.push(new memcell::NumMemcell(registers::topsp));
    }

    namespace
    {
    class MemcellCaller : public memcell::AvmMemcellVisitor {
    public:
        MemcellCaller() = default;
        ~MemcellCaller() = default;

        void visit_num_memcell(memcell::NumMemcell* memcell) const override {
            assert(memcell != nullptr);
        }

        void visit_string_memcell(memcell::StringMemcell* memcell) const
        override {
            assert(memcell != nullptr);

        }

        void visit_bool_memcell(memcell::BoolMemcell* memcell) const override {
            assert(memcell != nullptr);
        }

        void visit_table_memcell(memcell::TableMemcell* memcell) const override
        {
            assert(memcell != nullptr);
        }

        void visit_userfunc_memcell(memcell::UserfuncMemcell* memcell) const
        override {
            assert(memcell != nullptr);
        }

        void visit_libfunc_memcell(memcell::LibfuncMemcell* memcell) const
        override {
            assert(memcell != nullptr);
        }

        void visit_nill_memcell(memcell::NilMemcell* memcell) const override {
            assert(memcell != nullptr);
        }

        void visit_undef_memcell(memcell::UndefMemcell* memcell) const override
        {
            assert(memcell != nullptr);
        }
    };
    }

    void call_memcell(memcell::AvmMemcell* memcell) {
        PRECONDITION(memcell != nullptr);
        auto caller = new MemcellCaller();
        memcell->accept(caller);
    }

    }
}