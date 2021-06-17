#include "../include/library_functions.h"
#include "math.h"
#include <map>
#include <iostream>
#include "../../memcell/include/avm_memcell.h"
#include "../../signals/include/signals.h"
#include "../../registers/include/registers.h"
#include "../../memory/include/memory.h"

namespace avm
{
    namespace library_functions
    {

    namespace
    {
    void libfunc_print(void){
        unsigned totalactuals = cpu::topsp+3;
        for(unsigned i=0; i <totalactuals; i++){
        std::string arg = std::to_string(cpu::topsp+4+i);
        std::cout << arg <<'\n';
        }
    }

    void libfunc_typeof(void){
        unsigned totalactuals = cpu::topsp+3;
        if(totalactuals!=1)
            std::cout << "one argument not 'n' expected in typeof!" << totalactuals;
        else{
            //clean retval
            // set retval with the typeof the argument
            // cpu::retval
        }
    }

    void libfunc_input(void){

    }

    void libfunc_objectmemberkeys(void){

    }

    void libfunc_objecttotalmembers(void){

    }

    void libfunc_objectcopy(void){

    }

    void libfunc_totalarguments(void){
        
    }

    void libfunc_argument(void){

    }

    void libfunc_strtonum(void){
        unsigned totalactuals = cpu::topsp+3;
        if(totalactuals!=1)
            std::cout << "one argument not 'totalactuals' expected in typeof!" << totalactuals;
        else{
            std::string str = to_string(cpu::topsp+4+totalactuals);
            double num = std::stoi(str);
            //set retval
        }
    }

    void libfunc_sqrt(void){
        unsigned totalactuals = cpu::topsp+3;
        if(totalactuals!=1)
            std::cout << "one argument not 'totalactuals' expected in sqrt!" << totalactuals;
        else{
            double num = cpu::topsp+4+totalactuals;
            double sqrt1 = sqrt(num);
            //set retval
        }
    }

    void libfunc_cos(void){
        unsigned totalactuals = cpu::topsp+3;
        if(totalactuals!=1)
            std::cout << "one argument not 'totalactuals' expected in sqrt!" << totalactuals;
        else{
            double num = cpu::topsp+4+totalactuals;
            double sqrt1 = cos(num);
            //set retval
        }
    }

    void libfunc_sin(void){
        unsigned totalactuals = cpu::topsp+3;
        if(totalactuals!=1)
            std::cout << "one argument not 'totalactuals' expected in sqrt!" << totalactuals;
        else{
            double num = cpu::topsp+4+totalactuals;
            double sqrt1 = sin(num);
            //set retval
        }
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
    }
   
    void call_libfunc(std::string libfunc_id) {
        // if (!libfuncs_by_id.count(libfunc_id)) {
        //     signals::log_error(
        //         "unsupported lib func" + libfunc_id + " is called", std::cerr);
        // } else {
        //     auto lib_func = libfuncs_by_id[libfunc_id];
        //     registers::tops = registers::top;
        //     memory::total_actuals = 0;
        //     (*lib_func)();
        //     if (!signals::execution_finished)
                
        // }    
        
    }   

    }
}