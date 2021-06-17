#include "../include/library_functions.h"
#include "math.h"
#include <map>
#include <iostream>
#include "../../memcell/include/avm_memcell.h"
#include "../../signals/include/signals.h"
#include "../../registers/include/registers.h"
#include "../../memory/include/memory.h"
#include "../../exec/include/execute_function.h"

namespace avm
{
    namespace library_functions
    {

    namespace
    {
        unsigned total_actuals() {
            memory::stack_segment.get_envvalue(registers::topsp +
                AVM_NUMACTUALS_OFFSET);
        }

        memcell::AvmMemcell* get_actual(unsigned i) {
            assert(i <= total_actuals());
            return memory::stack_segment[registers::topsp + AVM_STACKENV_SIZE +
                1 + i];
        }

        void libfunc_print() {
            unsigned n = total_actuals();
            for (unsigned i = 0; i < n; i++) {
                std::cout << get_actual(i) << std::endl;
            }
        }

        void libfunc_typeof(){
            // unsigned totalactuals = cpu::topsp+3;
            // if(totalactuals!=1)
            //     std::cout << "one argument not 'n' expected in typeof!" << totalactuals;
            // else{
            //     //clean retval
            //     // set retval with the typeof the argument
            //     // cpu::retval
            // }
        }

        void libfunc_input(){

        }

        void libfunc_objectmemberkeys(){

        }

        void libfunc_objecttotalmembers(){

        }

        void libfunc_objectcopy(){

        }

        void libfunc_totalarguments(){
           
        }

        void libfunc_argument(){

        }

        void libfunc_strtonum(){
            // unsigned totalactuals = cpu::topsp+3;
            // if(totalactuals!=1)
            //     std::cout << "one argument not 'totalactuals' expected in typeof!" << totalactuals;
            // else{
            //     std::string str = to_string(cpu::topsp+4+totalactuals);
            //     double num = std::stoi(str);
            //     //set retval
            // }
        }

        void libfunc_sqrt(){
            // unsigned totalactuals = cpu::topsp+3;
            // if(totalactuals!=1)
            //     std::cout << "one argument not 'totalactuals' expected in sqrt!" << totalactuals;
            // else{
            //     double num = cpu::topsp+4+totalactuals;
            //     double sqrt1 = sqrt(num);
            //     //set retval
            // }
        }

        void libfunc_cos(){
            // unsigned totalactuals = cpu::topsp+3;
            // if(totalactuals!=1)
            //     std::cout << "one argument not 'totalactuals' expected in sqrt!" << totalactuals;
            // else{
            //     double num = cpu::topsp+4+totalactuals;
            //     double sqrt1 = cos(num);
            //     //set retval
            // }
        }

        void libfunc_sin(){
            // unsigned totalactuals = cpu::topsp+3;
            // if(totalactuals!=1)
            //     std::cout << "one argument not 'totalactuals' expected in sqrt!" << totalactuals;
            // else{
            //     double num = cpu::topsp+4+totalactuals;
            //     double sqrt1 = sin(num);
            //     //set retval
            // }
        }

        typedef void (*libfunc) ();

        std::map<std::string, libfunc> libfuncs_by_id = {
            {"print", libfunc_print},
            {"typeof", libfunc_typeof},
            {"input", libfunc_input},
            {"objectmemberkeys", libfunc_objectmemberkeys},
            {"objectcopy", libfunc_objectcopy},
            {"totalarguments", libfunc_totalarguments},
            {"argument", libfunc_argument},
            {"strtonum", libfunc_strtonum},
            {"sqrt", libfunc_sqrt},
            {"cos", libfunc_cos},
            {"sin", libfunc_sin}
        };

        bool unsupported_libfunc(std::string libfunc_id) {
            return !libfuncs_by_id.count(libfunc_id);
        }

        void execute_enterlibfunc() {
            registers::tops = registers::top;
            exec::total_actuals = 0;
        }

        void execute_libfunc(libfunc ptr) {
            assert (ptr != nullptr);
            (*ptr)();
        }

        void execute_exitlibfunc() {
            if (!signals::execution_finished)
                exec::execute_exitfunc();
        }
    }
   
    void call_libfunc(const std::string libfunc_id) {
        if (unsupported_libfunc(libfunc_id)) {
            signals::log_error(
                "unsupported lib func" + libfunc_id + " is called", std::cerr);
        } else {
            auto lib_func = libfuncs_by_id[libfunc_id];
            execute_enterlibfunc();
            execute_libfunc(lib_func);
            execute_exitlibfunc()          
        }    
    }   
    }
}