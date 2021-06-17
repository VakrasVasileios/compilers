#include "../include/execute_branch.h"
#include "../../registers/include/registers.h"

namespace avm
{
    namespace exec
    {
        void execute_jeq(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
            memcell::AvmMemcell* arg2) {
            PRECONDITION(res != nullptr);
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if (arg1 == arg2) {
                assert(dynamic_cast<memcell::NumMemcell*>(res) != nullptr);
                registers::pc = dynamic_cast<memcell::NumMemcell*>(res)->num_val();
            }
        }

        void execute_jne(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
            memcell::AvmMemcell* arg2) {
            PRECONDITION(res != nullptr);
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if (arg1 != arg2) {
                assert(dynamic_cast<memcell::NumMemcell*>(res) != nullptr);
                registers::pc = dynamic_cast<memcell::NumMemcell*>(res)->num_val();
            }
        }

        void execute_jgt(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
            memcell::AvmMemcell* arg2) {
            PRECONDITION(res != nullptr);
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if (arg1 > arg2) {
                assert(dynamic_cast<memcell::NumMemcell*>(res) != nullptr);
                registers::pc = dynamic_cast<memcell::NumMemcell*>(res)->num_val();
            }
        }

        void execute_jge(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
            memcell::AvmMemcell* arg2) {
            PRECONDITION(res != nullptr);
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if (arg1 >= arg2) {
                assert(dynamic_cast<memcell::NumMemcell*>(res) != nullptr);
                registers::pc = dynamic_cast<memcell::NumMemcell*>(res)->num_val();
            }
        }

        void execute_jlt(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
            memcell::AvmMemcell* arg2) {
            PRECONDITION(res != nullptr);
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if (arg1 < arg2) {
                assert(dynamic_cast<memcell::NumMemcell*>(res) != nullptr);
                registers::pc = dynamic_cast<memcell::NumMemcell*>(res)->num_val();
            }
        }

        void execute_jle(memcell::AvmMemcell* res, memcell::AvmMemcell* arg1, 
            memcell::AvmMemcell* arg2) {
            PRECONDITION(res != nullptr);
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if (arg1 <= arg2) {
                assert(dynamic_cast<memcell::NumMemcell*>(res) != nullptr);
                registers::pc = dynamic_cast<memcell::NumMemcell*>(res)->num_val();
            }
        }

        void execute_jmp(memcell::AvmMemcell* res) {
            PRECONDITION(res != nullptr);
            assert(dynamic_cast<memcell::NumMemcell*>(res) != nullptr);
            registers::pc = dynamic_cast<memcell::NumMemcell*>(res)->num_val();
        }
    }
}