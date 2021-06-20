#include "execute_function.h"
#include "../include/execute_assign.h"
#include "../../memory/include/memory.h"
#include "../../registers/include/registers.h"
#include "../../signals/include/signals.h"
#include "library_functions.h"

namespace avm
{
    namespace exec
    {
    unsigned total_actuals = 0;
    namespace
    {
        void call_saveenvironment() {
            memory::stack_segment.push_envvalue(total_actuals);
            //std::cout << registers::top << std::endl;
            memory::stack_segment.push_envvalue(registers::pc + 1);
            //std::cout << registers::top << std::endl;
            memory::stack_segment.push_envvalue(registers::top + total_actuals
                + 2);
            //std::cout << registers::top << std::endl;
            memory::stack_segment.push_envvalue(registers::topsp);
            //std::cout << registers::top << std::endl;
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
                call_libfunc(memcell->str_val());
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
                PRECONDITION(registers::pc > AVM_ENDING_PC);
                PRECONDITION(
                    memory::code_segment[registers::pc - 1]->get_opcode()
                        == target_code::ENTERFUNC_VM);
            }

            void visit_libfunc_memcell(memcell::LibfuncMemcell* memcell)
            override {
                assert(memcell != nullptr);
                call_libfunc(memcell->lib_func_val());
            }

            void visit_nill_memcell(memcell::NilMemcell* memcell) override {
                assert(memcell != nullptr);
                signals::log_error("Cannot bind NIL to function call",
                    std::cerr);
            }
            void visit_undef_memcell(memcell::UndefMemcell* memcell) override {
                assert(memcell != nullptr);
                signals::log_error(
                    "Cannot bind undifined value to function call", std::cerr);
            }
        };

        void call_memcell(memcell::AvmMemcell* memcell) {
            assert (memcell != nullptr);
            auto caller = new MemcellCaller();
            memcell->accept(caller);
        }

    } // namespace

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
            auto user_func =  userfunc_memcell_cast(memcell);
            assert(user_func != nullptr);
            return registers::pc == user_func->func_val();
        }
            
    }

    void execute_enterfunc(memcell::AvmMemcell* memcell) {
        PRECONDITION(memcell != nullptr);
        PRECONDITION(pc_is_legal(memcell));
        total_actuals = 0;
        auto func_info = memory::Constants::GetInstance().GetUserfunc(
            0);
        registers::topsp = registers::top;
        for (unsigned i = 0; i < func_info.local_count; i++) 
            memory::stack_segment.push(new memcell::UndefMemcell());
    }

    namespace
    {
        void restore_environment() {
            registers::top = memory::stack_segment.get_envvalue(
                registers::topsp + AVM_SAVEDTOP_OFFSET);   
               //std::cout << "recov top: " << registers::top << std::endl;
            registers::pc = memory::stack_segment.get_envvalue(
                registers::topsp + AVM_SAVEDPC_OFFSET);
                //std::cout << "recov pc: " << registers::pc << std::endl;
            registers::topsp = memory::stack_segment.get_envvalue(
                registers::topsp + AVM_SAVEDTOPSP_OFFSET);
                //std::cout << "recov topsp: " << registers::topsp << std::endl;
        }

        void garbage_collect(unsigned old_top) {
            assert (old_top >= 0);
            auto i = 0;
            int top = registers::top;
            while(++old_top < top) {
                //std::cout << "popping " << old_top << std::endl;
                delete memory::stack_segment[old_top];
            }
        }
    } // namespace

    void execute_exitfunc() {
        auto old_top = registers::top;
        restore_environment();
        // garbage_collect(old_top);
    }

    void execute_pusharg(memcell::AvmMemcell** memcell) {
        PRECONDITION(memcell);
        memory::stack_segment.push(new memcell::UndefMemcell());
        execute_assign(memory::stack_segment.top(), *memcell);
        POSTCONDITION(memory::stack_segment[registers::top] != nullptr);
        ++total_actuals;
    }
    
    } // namespace exec
} // namespace avm