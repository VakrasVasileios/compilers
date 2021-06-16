#include "execute_function.h"
#include "../../memory/include/memory.h"
#include "../../registers/include/registers.h"
#include "../../signals/include/signals.h"
#include "../../library_functions/include/library_functions.h"

namespace avm
{
    namespace cpu
    {

    unsigned total_actuals = 0;

    void save_environment() {
        memory::stack_segment.push(new memcell::NumMemcell(total_actuals));
        memory::stack_segment.push(new memcell::NumMemcell(registers::pc + 1));
        memory::stack_segment.push(new memcell::NumMemcell(
            registers::top + total_actuals + 2));
        memory::stack_segment.push(new memcell::NumMemcell(registers::topsp));
    }

        namespace
        {
        class MemcellCaller final : public memcell::AvmMemcellVisitor {
        public:
            MemcellCaller() = default;
            ~MemcellCaller() = default;

            void visit_num_memcell(memcell::NumMemcell* memcell) const override
            {
                assert(memcell != nullptr);
                signals::log_error("Cannot bind numerical value to function call",
                    std::cerr);
            }

            void visit_string_memcell(memcell::StringMemcell* memcell) const
            override {
                assert(memcell != nullptr);
                library_functions::call_libfunc(memcell->str_val());
            }

            void visit_bool_memcell(memcell::BoolMemcell* memcell) const
            override {
                assert(memcell != nullptr);
                signals::log_error("Cannot bind bool value to function call", std::cerr);
            }

            void visit_table_memcell(memcell::TableMemcell* memcell)
            const override {
                assert(memcell != nullptr);
                signals::log_error("Cannot bind table value to function call",
                    std::cerr);
            }

            void visit_userfunc_memcell(memcell::UserfuncMemcell* memcell) const
            override {
                assert(memcell != nullptr);
                registers::pc = memcell->func_val();
                POSTCONDITION(registers::pc < AVM_ENDING_PC);
                POSTCONDITION(memory::code_segment[registers::pc]->get_opcode()
                    == target_code::ENTERFUNC_VM);
            }

            void visit_libfunc_memcell(memcell::LibfuncMemcell* memcell) const
            override {
                assert(memcell != nullptr);
                library_functions::call_libfunc(memcell->lib_func_val());
            }

            void visit_nill_memcell(memcell::NilMemcell* memcell) const
            override {
                assert(memcell != nullptr);
                signals::log_error("Cannot bind NIL to function call", std::cerr);
            }

            void visit_undef_memcell(memcell::UndefMemcell* memcell) const
            override {
                assert(memcell != nullptr);
                signals::log_error("Cannot bind undifined value to function call",
                    std::cerr);
            }
        };
        }

    void call_memcell(memcell::AvmMemcell* memcell) {
        PRECONDITION(memcell != nullptr);
        auto caller = new MemcellCaller();
        memcell->accept(caller);
    }

    namespace
    {
        inline memcell::NumMemcell* num_memcell_cast(
            memcell::AvmMemcell* memcell) {
            auto result = dynamic_cast<memcell::NumMemcell*>(memcell);
            assert (result != nullptr);
            return result;
        }
    }

    unsigned get_envvalue(unsigned index) {
        PRECONDITION(index >= 0);
        auto memcell = memory::stack_segment[index];
        auto result = num_memcell_cast(memcell)->num_val();
        POSTCONDITION(result >= 0);

        return result;
    }

    }
}