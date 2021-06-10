#include "avm_memcell.h"

namespace virtual_machine 
{

double  
NumMemcell::num_val() const {
    return num_val_;    
}

void    
NumMemcell::set_num_val(const double _num_val) {
    num_val_ = _num_val;    
}

std::string
StringMemcell::str_val() const {
    return str_val_;    
}

void        
StringMemcell::set_str_val(const std::string str_val) {
    str_val_ = str_val;    
}

bool    
BoolMemcell::bool_val() const {
    return bool_val_;    
}

void    
BoolMemcell::set_bool_val(const bool val) {
    bool_val_ = val;    
}

AvmTable*   
TableMemcell::table_val() const {
    POSTCONDITION(table_val_ != nullptr);
    return table_val_;    
}

void        
TableMemcell::set_table_val(AvmTable* table) {
    INVARIANT(table_val_ != nullptr);
    PRECONDITION(table != nullptr);
    table_val_ = table;  
    INVARIANT(table_val_ != nullptr); 
}

AvmTable*   
TableMemcell::verify_avm_table(AvmTable* table) const {
    PRECONDITION(table != nullptr);
    return table;    
}

unsigned int    
UserfuncMemcell::func_val() const {
    return func_val_;    
}

void            
UserfuncMemcell::set_func_val(const unsigned int _func_val) {
    func_val_ = _func_val;    
}

std::string 
LibfuncMemcell::lib_func_val() const {
    return lib_func_val_;    
}

void        
LibfuncMemcell::set_lib_func_val(const std::string _lib_func_val) {
    lib_func_val_ = _lib_func_val;    
}

}