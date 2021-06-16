#include "../include/avm_memcell.h"

namespace avm 
{
    namespace memcell
    {
    bool  operator==(AvmMemcell const& lhs, AvmMemcell const& rhs) {
        return lhs.equals(rhs);
    }

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

    bool
    NumMemcell::to_bool() const {
        return num_val_ != 0;    
    }

    namespace 
    {
    NumMemcell const *num_memcell_cast(AvmMemcell const& memcell) {
        return dynamic_cast<NumMemcell const*>(&memcell);
    }  

    StringMemcell const *str_memcell_cast(AvmMemcell const& memcell) {
        return dynamic_cast<StringMemcell const*>(&memcell);
    }

    BoolMemcell const *bool_memcell_cast(AvmMemcell const& memcell) {
        return dynamic_cast<BoolMemcell const*>(&memcell);
    }

    TableMemcell const *table_memcell_cast(AvmMemcell const& memcell) {
        return dynamic_cast<TableMemcell const*>(&memcell);
    }

    UserfuncMemcell const *userfunc_memcell_cast(AvmMemcell const& memcell) {
        return dynamic_cast<UserfuncMemcell const*>(&memcell);
    }

    LibfuncMemcell const *libfunc_memcell_cast(AvmMemcell const& memcell) {
        return dynamic_cast<LibfuncMemcell const*>(&memcell);
    }

    NilMemcell const *nill_memcell_cast(AvmMemcell const& memcell) {
        return dynamic_cast<NilMemcell const*>(&memcell);
    }

    UndefMemcell const *undef_memcell_cast(AvmMemcell const& memcell) {
        return dynamic_cast<UndefMemcell const*>(&memcell);
    }

    }

    bool    
    NumMemcell::equals(AvmMemcell const& other) const {
        if (auto num_memcell = num_memcell_cast(other))
            return num_val_ == num_memcell->num_val();
        else    
            return to_bool() == other.to_bool();        
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
    StringMemcell::to_bool() const {
        return !str_val_.empty();    
    }

    bool    
    StringMemcell::equals(AvmMemcell const& other) const {
        if (auto str_memcell = str_memcell_cast(other))
            return str_val_ == str_memcell->str_val();
        else
            return to_bool() == other.to_bool();
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

    bool
    BoolMemcell::to_bool() const {
        return bool_val_;   
    }

    bool    
    BoolMemcell::equals(AvmMemcell const& other) const {
       return to_bool() == other.to_bool();
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

    bool        
    TableMemcell::equals(AvmMemcell const& other) const {
        if (auto table_memcell = table_memcell_cast(other))
            return table_val_ == table_memcell->table_val();
        else    
            return to_bool() == other.to_bool(); 
    }

    bool
    TableMemcell::to_bool() const {
        return true;   
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

    bool            
    UserfuncMemcell::equals(AvmMemcell const& other) const {
       if (auto userfunc_memcell = userfunc_memcell_cast(other))
            return func_val_ == userfunc_memcell->func_val();
        else
            return to_bool() == other.to_bool();
    }

    bool
    UserfuncMemcell::to_bool() const {
        return true;   
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

    bool        
    LibfuncMemcell::equals(AvmMemcell const& other) const {
        if (auto libfunc_memcell = libfunc_memcell_cast(other))
            return lib_func_val_ == libfunc_memcell->lib_func_val();
        else
            return to_bool() == other.to_bool();
    }

    bool
    LibfuncMemcell::to_bool() const {
        return true;   
    }

    void
    NilMemcell::accept(const AvmMemcellVisitor* visitor) {
        PRECONDITION(visitor != nullptr);
        visitor->visit_nill_memcell(this);
    }

    bool        
    NilMemcell::equals(AvmMemcell const& other) const {
        return to_bool() == other.to_bool();  
    }

    bool
    NilMemcell::to_bool() const {
        return false;   
    }

    void
    UndefMemcell::accept(const AvmMemcellVisitor* visitor) {
        PRECONDITION(visitor != nullptr);
        visitor->visit_undef_memcell(this);
    }

    bool        
    UndefMemcell::equals(AvmMemcell const& other) const {
        return to_bool() == other.to_bool();    
    }

    bool
    UndefMemcell::to_bool() const {
        return false;   
    }
    }
}