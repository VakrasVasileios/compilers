#ifndef LIBRARY_FUNCTIONS_H
#define LIBRARY_FUNCTIONS_H

#include <string>

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
    }
}


#endif