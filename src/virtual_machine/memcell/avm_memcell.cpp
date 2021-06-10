#include "../../../include/virtual_machine/memcell/avm_memcell.h"

namespace virtual_machine 
{
    namespace memcell
    {
    double  
    NumMemcell::num_val() const {
        return num_val_;    
    }

    void    
    NumMemcell::set_num_val(const double _num_val) {
        num_val_ = _num_val;    
    }

    void
    NumMemcell::accept(const AvmMemcellVisitor* visitor) {
        PRECONDITION(visitor != nullptr);
        visitor->visit_num_memcell(this);
    }

    std::string
    StringMemcell::str_val() const {
        return str_val_;    
    }

    void        
    StringMemcell::set_str_val(const std::string str_val) {
        str_val_ = str_val;    
    }

    void
    StringMemcell::accept(const AvmMemcellVisitor* visitor) {
        PRECONDITION(visitor != nullptr);
        visitor->visit_string_memcell(this);
    }

    bool    
    BoolMemcell::bool_val() const {
        return bool_val_;    
    }

    void    
    BoolMemcell::set_bool_val(const bool val) {
        bool_val_ = val;    
    }

    void
    BoolMemcell::accept(const AvmMemcellVisitor* visitor) {
        PRECONDITION(visitor != nullptr);
        visitor->visit_bool_memcell(this);
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

    void
    TableMemcell::accept(const AvmMemcellVisitor* visitor) {
        PRECONDITION(visitor != nullptr);
        visitor->visit_table_memcell(this);
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

    void
    UserfuncMemcell::accept(const AvmMemcellVisitor* visitor) {
        PRECONDITION(visitor != nullptr);
        visitor->visit_userfunc_memcell(this);
    }

    std::string 
    LibfuncMemcell::lib_func_val() const {
        return lib_func_val_;    
    }

    void        
    LibfuncMemcell::set_lib_func_val(const std::string _lib_func_val) {
        lib_func_val_ = _lib_func_val;    
    }

    void
    LibfuncMemcell::accept(const AvmMemcellVisitor* visitor) {
        PRECONDITION(visitor != nullptr);
        visitor->visit_libfunc_memcell(this);
    }

    void
    NilMemcell::accept(const AvmMemcellVisitor* visitor) {
        PRECONDITION(visitor != nullptr);
        visitor->visit_nill_memcell(this);
    }

    void
    UndefMemcell::accept(const AvmMemcellVisitor* visitor) {
        PRECONDITION(visitor != nullptr);
        visitor->visit_undef_memcell(this);
    }
    }
}