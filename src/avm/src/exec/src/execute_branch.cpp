#include "../include/execute_branch.h"
#include "../../registers/include/registers.h"

namespace avm
{
    namespace exec
    {
        void execute_jeq(const memcell::AvmMemcell* res, 
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2) {
            PRECONDITION(res != nullptr);
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if ((*arg1 == *arg2)->to_bool()) {
                assert(dynamic_cast<const memcell::NumMemcell*>(res) != nullptr);
                registers::pc = dynamic_cast<const memcell::NumMemcell*>(res)->num_val();
            }
        }

        void execute_jne(const memcell::AvmMemcell* res,
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2) {
            PRECONDITION(res != nullptr);
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if ((*arg1 != *arg2)->to_bool()) {
                assert(dynamic_cast<const memcell::NumMemcell*>(res) != nullptr);
                registers::pc = dynamic_cast<const memcell::NumMemcell*>(res)->num_val();
            }
        }

        void execute_jgt(const memcell::AvmMemcell* res,
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2) {
            PRECONDITION(res != nullptr);
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if ((*arg1 > *arg2)->to_bool()) {
                assert(dynamic_cast<const memcell::NumMemcell*>(res) != nullptr);
                registers::pc = dynamic_cast<const memcell::NumMemcell*>(res)->num_val();
            }
        }

        void execute_jge(const memcell::AvmMemcell* res, 
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2) {
            PRECONDITION(res != nullptr);
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if ((*arg1 >= *arg2)->to_bool()) {
                assert(dynamic_cast<const memcell::NumMemcell*>(res) != nullptr);
                registers::pc = dynamic_cast<const memcell::NumMemcell*>(res)->num_val();
            }
        }

        void execute_jlt(const memcell::AvmMemcell* res,
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2) {
            PRECONDITION(res != nullptr);
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if ((*arg1 < *arg2)->to_bool()) {
                assert(dynamic_cast<const memcell::NumMemcell*>(res) != nullptr);
                registers::pc = dynamic_cast<const memcell::NumMemcell*>(res)->num_val();
            }
        }

        void execute_jle(const memcell::AvmMemcell* res, 
            const memcell::AvmMemcell* arg1, const memcell::AvmMemcell* arg2) {
            PRECONDITION(res != nullptr);
            PRECONDITION(arg1 != nullptr);
            PRECONDITION(arg2 != nullptr);
            if ((*arg1 <= *arg2)->to_bool()) {
                assert(dynamic_cast<const memcell::NumMemcell*>(res) != nullptr);
                registers::pc = dynamic_cast<const memcell::NumMemcell*>(res)->num_val();
            }
        }

        void execute_jmp(const memcell::AvmMemcell* res) {
            PRECONDITION(res != nullptr);
            assert(dynamic_cast<const memcell::NumMemcell*>(res) != nullptr);
            registers::pc = dynamic_cast<const memcell::NumMemcell*>(res)->num_val();
        }
    }
}