#include "execute_function.h"
#include "../../memory/include/memory.h"
#include "../../registers/include/registers.h"
#include "../../signals/include/signals.h"
#include "../../library_functions/include/library_functions.h"

namespace avm
{
    namespace exec
    {
    unsigned total_actuals = 0;
    namespace
    {
        void call_saveenvironment() {
            memory::stack_segment.push_envvalue(total_actuals);
            memory::stack_segment.push_envvalue(registers::pc + 1);
            memory::stack_segment.push_envvalue(registers::top + total_actuals 
                + 2);
            memory::stack_segment.push_envvalue(registers::topsp);   
        }

        class MemcellCaller final : public memcell::AvmMemcellVisitor {
        public:
            MemcellCaller() = default;
            ~MemcellCaller() = default;

            void visit_num_memcell(memcell::NumMemcell* memcell) override {
                assert(memcell != nullptr);
                signals::log_error(
                    "Cannot bind numerical value to function call", std::cerr);
            }

            void visit_string_memcell(memcell::StringMemcell* memcell) override
            {
                assert(memcell != nullptr);
                library_functions::call_libfunc(memcell->str_val());
            }

            void visit_bool_memcell(memcell::BoolMemcell* memcell) override {
                assert(memcell != nullptr);
                signals::log_error("Cannot bind bool value to function call",
                    std::cerr);
            }

            void visit_table_memcell(memcell::TableMemcell* memcell) override {
                assert(memcell != nullptr);
                signals::log_error("Cannot bind table value to function call",
                    std::cerr);
            }

            void visit_userfunc_memcell(memcell::UserfuncMemcell* memcell)
            override {
                assert(memcell != nullptr);
                registers::pc = memcell->func_val();
                POSTCONDITION(registers::pc < AVM_ENDING_PC);
                POSTCONDITION(memory::code_segment[registers::pc]->get_opcode()
                    == target_code::ENTERFUNC_VM);
            }

            void visit_libfunc_memcell(memcell::LibfuncMemcell* memcell)
            override {
                assert(memcell != nullptr);
                library_functions::call_libfunc(memcell->lib_func_val());
            }

            void visit_nill_memcell(memcell::NilMemcell* memcell) override {
                assert(memcell != nullptr);
                signals::log_error("Cannot bind NIL to function call",
                    std::cerr);
            }
        };

        void call_memcell(memcell::AvmMemcell* memcell) {
            assert (memcell != nullptr);
            auto caller = new MemcellCaller();
            memcell->accept(caller);
        }

    }

    void execute_callfunc(memcell::AvmMemcell* memcell) {
        PRECONDITION(memcell != nullptr);
        call_saveenvironment();
        call_memcell(memcell);
    }

    namespace 
    {
        memcell::UserfuncMemcell* userfunc_memcell_cast(
            memcell::AvmMemcell* memcell) {
                assert(memcell != nullptr);
                auto userfunc = 
                    dynamic_cast<memcell::UserfuncMemcell*>(memcell);
                PRECONDITION(userfunc != nullptr);
                return userfunc;    
        }

        bool pc_is_legal(memcell::AvmMemcell* memcell) {
            assert(memcell != nullptr);
            return registers::pc == userfunc_memcell_cast(memcell)->func_val();
        }
            
    }

    void execute_enterfunc(memcell::AvmMemcell* memcell) {
        PRECONDITION(memcell != nullptr);
        PRECONDITION(pc_is_legal(memcell));
        total_actuals = 0;
        auto func_info = memory::Constants::GetInstance().GetUserfunc(
            registers::pc);
        registers::topsp = registers::top;
        registers::top = registers::top - func_info.local_count;
    }

    namespace
    {
        void restore_environment() {
            registers::top = memory::stack_segment.get_envvalue(
                registers::topsp + AVM_SAVEDTOP_OFFSET);    
            registers::pc = memory::stack_segment.get_envvalue(
                registers::topsp + AVM_SAVEDPC_OFFSET);  
            registers::topsp = memory::stack_segment.get_envvalue(
                registers::topsp + AVM_SAVEDTOPSP_OFFSET); 
        }

        void garbage_collect(unsigned old_top) {
            assert (old_top >= 0);
            while(++old_top <= registers::top)
                memory::stack_segment.pop(); 
        }
    }

    void execute_exitfunc() {
        auto old_top = registers::top;
        restore_environment();
        garbage_collect(old_top);      
    }

    void execute_pusharg(memcell::AvmMemcell* memcell) {
        PRECONDITION(memcell);
        memory::stack_segment.push(memcell);
        ++total_actuals;
    }
    
    }
}