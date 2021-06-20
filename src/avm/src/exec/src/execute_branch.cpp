#include "../include/execute_branch.h"
#include "../../registers/include/registers.h"
#include "../../memory/include/memory.h"

namespace avm
{
    namespace exec
    {
        void execute_jeq(const unsigned res, 
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2) {
            PRECONDITION(res < memory::code_segment.size());
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if (*arg1 == *arg2) {
                registers::pc = res;
            }
        }

        void execute_jne(const unsigned res,
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2) {
            PRECONDITION(res < memory::code_segment.size());
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if (*arg1 != *arg2) {
                registers::pc = res;
            }
        }

        void execute_jgt(const unsigned res,
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2) {
            PRECONDITION(res < memory::code_segment.size());
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if (*arg1 > *arg2) {
                registers::pc = res;
            }
        }

        void execute_jge(const unsigned res, 
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2) {
            PRECONDITION(res < memory::code_segment.size());
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if (*arg1 >= *arg2) {
                registers::pc = res;
            }
        }

        void execute_jlt(const unsigned res,
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2) {
            PRECONDITION(res < memory::code_segment.size());
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if (*arg1 < *arg2) {
                registers::pc = res;
            }
        }

        void execute_jle(const unsigned res, 
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2) {
            PRECONDITION(res < memory::code_segment.size());
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if (*arg1 <= *arg2) {
                registers::pc = res;
            }
        }

        void execute_jmp(const unsigned res) {
            PRECONDITION(res < memory::code_segment.size());
            registers::pc = res;
        }
    }
}