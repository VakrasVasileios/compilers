#ifndef LIBRARY_FUNCTIONS_H
#define LIBRARY_FUNCTIONS_H

#include <iostream>
#include <string>
#include "../../memcell/include/avm_memcell.h"

namespace avm
{
    namespace library_functions
    {
    void call_libfunc(std::string libfunc_id);
    // void avm_libfunc_call(string id){
    //        memcell::lib_func_data f = memcell::lib_func_data::lib_func_data(id);
    //         /*if(f==NULL){
    //              std::cout << "unsupported lib func 'id' is called" << id;            }
    //         else{
    //             cpu::topsp= cpu::top;
    //             //f*(); call library function
    //         }*/
    //     }

    //     void libfunc_print(void){
    //         unsigned totalactuals = cpu::topsp+3;
    //         for(unsigned i=0; i <totalactuals; i++){
    //         std::string arg = std::to_string(cpu::topsp+4+i);
    //         std::cout << arg <<'\n';
    //         }
    //     }
    //     void libfunc_typeof(void){
    //         unsigned totalactuals = cpu::topsp+3;
    //         if(totalactuals!=1)
    //             std::cout << "one argument not 'n' expected in typeof!" << totalactuals;
    //         else{
    //             //clean retval
    //             // set retval with the typeof the argument
    //            // cpu::retval
    //         }
    //     }
    //     void libfunc_input(void){

    //     }
    //     void libfunc_objectmemberkeys(void){

    //     }
    //     void libfunc_objecttotalmembers(void){

    //     }
    //      void libfunc_objectcopy(void){

    //     }
    //      void libfunc_totalarguments(void){
            
    //     }
    //      void libfunc_argument(void){

    //     }
    //     void libfunc_strtonum(void){
    //         unsigned totalactuals = cpu::topsp+3;
    //         if(totalactuals!=1)
    //             std::cout << "one argument not 'totalactuals' expected in typeof!" << totalactuals;
    //         else{
    //             std::string str = to_string(cpu::topsp+4+totalactuals);
    //             double num = std::stoi(str);
    //             //set retval
    //         }
    //     }
    //     void libfunc_sqrt(void){
    //         unsigned totalactuals = cpu::topsp+3;
    //         if(totalactuals!=1)
    //             std::cout << "one argument not 'totalactuals' expected in sqrt!" << totalactuals;
    //         else{
    //             double num = cpu::topsp+4+totalactuals;
    //             double sqrt1 = sqrt(num);
    //             //set retval
    //         }
    //     }
    //     void libfunc_cos(void){
    //         unsigned totalactuals = cpu::topsp+3;
    //         if(totalactuals!=1)
    //             std::cout << "one argument not 'totalactuals' expected in sqrt!" << totalactuals;
    //         else{
    //             double num = cpu::topsp+4+totalactuals;
    //             double sqrt1 = cos(num);
    //             //set retval
    //         }
    //     }
    //     void libfunc_sin(void){
    //          unsigned totalactuals = cpu::topsp+3;
    //         if(totalactuals!=1)
    //             std::cout << "one argument not 'totalactuals' expected in sqrt!" << totalactuals;
    //         else{
    //             double num = cpu::topsp+4+totalactuals;
    //             double sqrt1 = sin(num);
    //             //set retval
    //         }
    //     }
    }
}


#endif