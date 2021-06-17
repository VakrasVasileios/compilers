#include "../include/avm_memcell.h"
#include <iostream>
#include <math.h>

namespace avm 
{
    namespace memcell
    {

        //--------------AvmMemcell--------------//
        std::ostream& operator << (std::ostream& os,
                const AvmMemcell& memcell) {
            return memcell.log(os);    
        }

        AvmMemcell*
        operator==(AvmMemcell const& lhs, AvmMemcell const& rhs) {
            return lhs.equals(rhs);
        }

        AvmMemcell*
        AvmMemcell::operator+(AvmMemcell const& other) {
            return add(other);
        }

        AvmMemcell*
        AvmMemcell::operator-(AvmMemcell const& other) {
            return sub(other);
        }

        AvmMemcell*
        AvmMemcell::operator*(AvmMemcell const& other) {
            return mul(other);
        }

        AvmMemcell*
        AvmMemcell::operator/(AvmMemcell const& other) {
            return div(other);
        }

        AvmMemcell*
        AvmMemcell::operator%(AvmMemcell const& other) {
            return mod(other);
        }

        AvmMemcell*
        AvmMemcell::operator>(AvmMemcell const& other) {
            return gt(other);
        }

        AvmMemcell*
        AvmMemcell::operator>=(AvmMemcell const& other) {
            return geq(other);
        }

        AvmMemcell*
        AvmMemcell::operator<(AvmMemcell const& other) {
            return lt(other);
        }

        AvmMemcell*
        AvmMemcell::operator<=(AvmMemcell const& other) {
            return leq(other);
        }

        //--------------AvmMemcell--------------//

        //--------------NumMemcell--------------//
        double  
        NumMemcell::num_val() const {
            return num_val_;    
        }

        void    
        NumMemcell::set_num_val(const double _num_val) {
            num_val_ = _num_val;    
        }

        void
        NumMemcell::accept(AvmMemcellVisitor* visitor) {
            PRECONDITION(visitor != nullptr);
            visitor->visit_num_memcell(this);
        }

        bool
        NumMemcell::to_bool() const {
            return num_val_ != 0;    
        }

        std::ostream&   
        NumMemcell::log(std::ostream& os) const {
            return os << num_val_;    
        }

        AvmMemcell*    
        NumMemcell::equals(AvmMemcell const& other) const {
            if (auto num_memcell = num_memcell_cast(other))
                return new BoolMemcell(num_val_ == num_memcell->num_val());
            else    
                return new BoolMemcell(to_bool() == other.to_bool());        
        }

        AvmMemcell*
        NumMemcell::add(AvmMemcell const& other) const {
            if (auto num = num_memcell_cast(other)) {
                NumMemcell* ret = new NumMemcell();
                ret->num_val_ = num_val_ + num->num_val_;
                return ret;
            }
            else if (auto str = str_memcell_cast(other)) {
                StringMemcell* ret = new StringMemcell("");
                std::string tmp(std::to_string(num_val_));
                ret->set_str_val(tmp+str->str_val());
                return ret;
            }
            else {
                std::cerr << "Invalid operation \'+\'" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        AvmMemcell*
        NumMemcell::sub(AvmMemcell const& other) const {
            NumMemcell* ret = new NumMemcell();
            if (auto num = num_memcell_cast(other)) {
                ret->num_val_ = num_val_ - num->num_val_;
                return ret;
            }
            else {
                std::cerr << "Second operand is not a number" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        AvmMemcell*
        NumMemcell::mul(AvmMemcell const& other) const {
            NumMemcell* ret = new NumMemcell();
            if (auto num = num_memcell_cast(other)) {
                ret->num_val_ = num_val_ * num->num_val_;
                return ret;
            }
            else {
                std::cerr << "Second operand is not a number" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        AvmMemcell*
        NumMemcell::div(AvmMemcell const& other) const {
            NumMemcell* ret = new NumMemcell();
            if (auto num = num_memcell_cast(other)) {
                if (num->num_val_ != 0) {
                    ret->num_val_ = num_val_ / num->num_val_;
                    return ret;
                }
                else {
                    std::cerr << "Division with zero" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            else {
                std::cerr << "Second operand is not a number" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        AvmMemcell*
        NumMemcell::mod(AvmMemcell const& other) const {
            NumMemcell* ret = new NumMemcell();
            if (auto num = num_memcell_cast(other)) {
                if (num->num_val_ != 0) {
                    ret->num_val_ = fmod(num_val_, num->num_val_);
                    return ret;
                }
                else {
                    std::cerr << "Modulation with zero" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            else {
                std::cerr << "Second operand is not a number" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        AvmMemcell*
        NumMemcell::gt(AvmMemcell const& other) const {
            BoolMemcell* ret = new BoolMemcell();
            if (auto num = num_memcell_cast(other)) {
                ret->set_bool_val(num_val_ > num->num_val_);
                return ret;
            }
            else {
                std::cerr << "Invalid '>' operation with non-numeric rh operand" << std::endl;
            }
        }

        AvmMemcell*
        NumMemcell::geq(AvmMemcell const& other) const {
            BoolMemcell* ret = new BoolMemcell();
            if (auto num = num_memcell_cast(other)) {
                ret->set_bool_val(num_val_ >= num->num_val_);
                return ret;
            }
            else {
                std::cerr << "Invalid '>=' operation with non-numeric rh operand" << std::endl;
            }
        }

        AvmMemcell*
        NumMemcell::lt(AvmMemcell const& other) const {
            BoolMemcell* ret = new BoolMemcell();
            if (auto num = num_memcell_cast(other)) {
                ret->set_bool_val(num_val_ < num->num_val_);
                return ret;
            }
            else {
                std::cerr << "Invalid '<' operation with non-numeric rh operand" << std::endl;
            }
        }

        AvmMemcell*
        NumMemcell::leq(AvmMemcell const& other) const {
            BoolMemcell* ret = new BoolMemcell();
            if (auto num = num_memcell_cast(other)) {
                ret->set_bool_val(num_val_ <= num->num_val_);
                return ret;
            }
            else {
                std::cerr << "Invalid '<=' operation with non-numeric rh operand" << std::endl;
            }
        }
        //--------------NumMemcell--------------//

        //--------------StringMemcell--------------//
        std::string
        StringMemcell::str_val() const {
            return str_val_;    
        }

        void        
        StringMemcell::set_str_val(const std::string str_val) {
            str_val_ = str_val;    
        }

        void
        StringMemcell::accept(AvmMemcellVisitor* visitor) {
            PRECONDITION(visitor != nullptr);
            visitor->visit_string_memcell(this);
        }

        bool
        StringMemcell::to_bool() const {
            return !str_val_.empty();    
        }

        std::ostream&   
        StringMemcell::log(std::ostream& os) const {
            return os << str_val_;    
        }

        AvmMemcell*    
        StringMemcell::equals(AvmMemcell const& other) const {
            if (auto str_memcell = str_memcell_cast(other))
                return new BoolMemcell(str_val_ == str_memcell->str_val());
            else
                return new BoolMemcell(to_bool() == other.to_bool());
        }

        AvmMemcell*
        StringMemcell::add(AvmMemcell const& other) const {
            StringMemcell* ret = new StringMemcell("");
            if (auto str = str_memcell_cast(other)) {
                ret->str_val_ += str_val_ + str->str_val_;
                return ret;
            }
            else if (auto num = num_memcell_cast(other)) {
                std::string tmp = str_val_;
                tmp += num->num_val();
                ret->str_val_ += tmp;
                return ret;
            }
            else {
                std::cerr << "Invalid operation \'+\'" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        AvmMemcell*
        StringMemcell::sub(AvmMemcell const& other) const {
            std::cerr << "Invalid '-' with string operand" << std::endl;
        }

        AvmMemcell*
        StringMemcell::mul(AvmMemcell const& other) const {
            std::cerr << "Invalid '*' with string operand" << std::endl;
        }

        AvmMemcell*
        StringMemcell::div(AvmMemcell const& other) const {
            std::cerr << "Invalid '/' with string operand" << std::endl;
        }

        AvmMemcell*
        StringMemcell::mod(AvmMemcell const& other) const {
            std::cerr << "Invalid '%' with string operand" << std::endl;
        }

        AvmMemcell*
        StringMemcell::gt(AvmMemcell const& other) const {
            std::cerr << "Invalid '>' with string operand" << std::endl;
        }

        AvmMemcell*
        StringMemcell::geq(AvmMemcell const& other) const {
            std::cerr << "Invalid '>=' with string operand" << std::endl;
        }

        AvmMemcell*
        StringMemcell::lt(AvmMemcell const& other) const {
            std::cerr << "Invalid '<' with string operand" << std::endl;
        }

        AvmMemcell*
        StringMemcell::leq(AvmMemcell const& other) const {
            std::cerr << "Invalid '<=' with string operand" << std::endl;
        }
        //--------------StringMemcell--------------//

        //--------------BoolMemcell--------------//
        bool    
        BoolMemcell::bool_val() const {
            return bool_val_;    
        }

        void    
        BoolMemcell::set_bool_val(const bool val) {
            bool_val_ = val;    
        }

        void
        BoolMemcell::accept(AvmMemcellVisitor* visitor) {
            PRECONDITION(visitor != nullptr);
            visitor->visit_bool_memcell(this);
        }

        bool
        BoolMemcell::to_bool() const {
            return bool_val_;   
        }

        std::ostream&   
        BoolMemcell::log(std::ostream& os) const {
            return os << bool_val_;    
        }

        AvmMemcell*    
        BoolMemcell::equals(AvmMemcell const& other) const {
        return new BoolMemcell(to_bool() == other.to_bool());
        }

        AvmMemcell*
        BoolMemcell::add(AvmMemcell const& other) const {
            std::cerr << "Invalid '+' with bool operand" << std::endl;
        }

        AvmMemcell*
        BoolMemcell::sub(AvmMemcell const& other) const {
            std::cerr << "Invalid '-' with bool operand" << std::endl;
        }

        AvmMemcell*
        BoolMemcell::mul(AvmMemcell const& other) const {
            std::cerr << "Invalid '*' with bool operand" << std::endl;
        }

        AvmMemcell*
        BoolMemcell::div(AvmMemcell const& other) const {
            std::cerr << "Invalid '/' with bool operand" << std::endl;
        }

        AvmMemcell*
        BoolMemcell::mod(AvmMemcell const& other) const {
            std::cerr << "Invalid '%' with bool operand" << std::endl;
        }
        //--------------BoolMemcell--------------//

        //--------------TableMemcell--------------//
        bool                
        TableMemcell::contains(AvmMemcell* key) const {
            PRECONDITION(key != nullptr);
            return table_val_.count(key);
        }

        AvmMemcell* 
        TableMemcell::get_elem(AvmMemcell* key) const {
            PRECONDITION(contains(key));
            return table_val_.at(key);
        }

        void        
        TableMemcell::set_elem(AvmMemcell* key, AvmMemcell* value) {
            PRECONDITION(key != nullptr);
            PRECONDITION(value != nullptr);
            table_val_.insert({key, value});
        }

        void
        TableMemcell::accept(AvmMemcellVisitor* visitor) {
            PRECONDITION(visitor != nullptr);
            visitor->visit_table_memcell(this);
        }

        AvmMemcell*        
        TableMemcell::equals(AvmMemcell const& other) const {
            if (auto table_memcell = table_memcell_cast(other))
                // return table_val_ == table_memcell->table_val(); TODO: check maps
            // else    
            //     return to_bool() == other.to_bool(); 
            return new BoolMemcell(to_bool() == other.to_bool());
        }

        bool
        TableMemcell::to_bool() const {
            return true;   
        }

        std::ostream&   
        TableMemcell::log(std::ostream& os) const {
            return os; //TODO    
        }

        AvmMemcell*
        TableMemcell::add(AvmMemcell const& other) const {
            std::cerr << "Invalid '+' with table operand" << std::endl;
        }

        AvmMemcell*
        TableMemcell::sub(AvmMemcell const& other) const {
            std::cerr << "Invalid '-' with table operand" << std::endl;
        }

        AvmMemcell*
        TableMemcell::mul(AvmMemcell const& other) const {
            std::cerr << "Invalid '*' with table operand" << std::endl;
        }

        AvmMemcell*
        TableMemcell::div(AvmMemcell const& other) const {
            std::cerr << "Invalid '/' with table operand" << std::endl;
        }

        AvmMemcell*
        TableMemcell::mod(AvmMemcell const& other) const {
            std::cerr << "Invalid '%' with table operand" << std::endl;
        }
        //--------------TableMemcell--------------//

        //--------------UserfuncMemcell--------------//
        unsigned int    
        UserfuncMemcell::func_val() const {
            return func_val_;    
        }

        void            
        UserfuncMemcell::set_func_val(const unsigned int _func_val) {
            func_val_ = _func_val;    
        }

        void
        UserfuncMemcell::accept(AvmMemcellVisitor* visitor) {
            PRECONDITION(visitor != nullptr);
            visitor->visit_userfunc_memcell(this);
        }

        AvmMemcell*            
        UserfuncMemcell::equals(AvmMemcell const& other) const {
        if (auto userfunc_memcell = userfunc_memcell_cast(other))
                return new BoolMemcell(func_val_ == userfunc_memcell->func_val());
            else
                return new BoolMemcell(to_bool() == other.to_bool());
        }

        bool
        UserfuncMemcell::to_bool() const {
            return true;   
        }

        std::ostream&   
        UserfuncMemcell::log(std::ostream& os) const {
            return os << func_val_; //TODO???    
        }

        AvmMemcell*
        UserfuncMemcell::add(AvmMemcell const& other) const {
            std::cerr << "Invalid '+' with user function operand" << std::endl;
        }

        AvmMemcell*
        UserfuncMemcell::sub(AvmMemcell const& other) const {
            std::cerr << "Invalid '-' with user function operand" << std::endl;
        }

        AvmMemcell*
        UserfuncMemcell::mul(AvmMemcell const& other) const {
            std::cerr << "Invalid '*' with user function operand" << std::endl;
        }

        AvmMemcell*
        UserfuncMemcell::div(AvmMemcell const& other) const {
            std::cerr << "Invalid '/' with user function operand" << std::endl;
        }

        AvmMemcell*
        UserfuncMemcell::mod(AvmMemcell const& other) const {
            std::cerr << "Invalid '%' with user function operand" << std::endl;
        }
        //--------------UserfuncMemcell--------------//

        //--------------LibfuncMemcell--------------//
        std::string 
        LibfuncMemcell::lib_func_val() const {
            return lib_func_val_;    
        }

        void        
        LibfuncMemcell::set_lib_func_val(const std::string _lib_func_val) {
            lib_func_val_ = _lib_func_val;    
        }

        void
        LibfuncMemcell::accept(AvmMemcellVisitor* visitor) {
            PRECONDITION(visitor != nullptr);
            visitor->visit_libfunc_memcell(this);
        }

        AvmMemcell*        
        LibfuncMemcell::equals(AvmMemcell const& other) const {
            if (auto libfunc_memcell = libfunc_memcell_cast(other))
                return new BoolMemcell(lib_func_val_ == libfunc_memcell->lib_func_val());
            else
                return new BoolMemcell(to_bool() == other.to_bool());
        }

        bool
        LibfuncMemcell::to_bool() const {
            return true;   
        }

        std::ostream&   
        LibfuncMemcell::log(std::ostream& os) const {
            return os << lib_func_val_; // TODO??    
        }

        AvmMemcell*
        LibfuncMemcell::add(AvmMemcell const& other) const {
            std::cerr << "Invalid '+' with library function operand" << std::endl;
        }

        AvmMemcell*
        LibfuncMemcell::sub(AvmMemcell const& other) const {
            std::cerr << "Invalid '-' with library function operand" << std::endl;
        }

        AvmMemcell*
        LibfuncMemcell::mul(AvmMemcell const& other) const {
            std::cerr << "Invalid '*' with library function operand" << std::endl;
        }

        AvmMemcell*
        LibfuncMemcell::div(AvmMemcell const& other) const {
            std::cerr << "Invalid '/' with library function operand" << std::endl;
        }

        AvmMemcell*
        LibfuncMemcell::mod(AvmMemcell const& other) const {
            std::cerr << "Invalid '%' with library function operand" << std::endl;
        }
        //--------------LibfuncMemcell--------------//

        //--------------NilMemcell--------------//
        void
        NilMemcell::accept(AvmMemcellVisitor* visitor) {
            PRECONDITION(visitor != nullptr);
            visitor->visit_nill_memcell(this);
        }

        AvmMemcell*
        NilMemcell::equals(AvmMemcell const& other) const {
            return new BoolMemcell(to_bool() == other.to_bool());  
        }

        bool
        NilMemcell::to_bool() const {
            return false;   
        }

        std::ostream& 
        NilMemcell::log(std::ostream& os) const {
            return os << "NIL";    
        }

        AvmMemcell*
        NilMemcell::add(AvmMemcell const& other) const {
            std::cerr << "Invalid '+' with nil operand" << std::endl;
        }

        AvmMemcell*
        NilMemcell::sub(AvmMemcell const& other) const {
            std::cerr << "Invalid '-' with nil operand" << std::endl;
        }

        AvmMemcell*
        NilMemcell::mul(AvmMemcell const& other) const {
            std::cerr << "Invalid '*' with nil operand" << std::endl;
        }

        AvmMemcell*
        NilMemcell::div(AvmMemcell const& other) const {
            std::cerr << "Invalid '/' with nil operand" << std::endl;
        }

        AvmMemcell*
        NilMemcell::mod(AvmMemcell const& other) const {
            std::cerr << "Invalid '%' with nil operand" << std::endl;
        }
        //--------------NilMemcell--------------//

        //--------------UndefMemcell--------------//
        void
        UndefMemcell::accept(AvmMemcellVisitor* visitor) {
            PRECONDITION(visitor != nullptr);
            visitor->visit_undef_memcell(this);
        }

        AvmMemcell*        
        UndefMemcell::equals(AvmMemcell const& other) const {
            return new BoolMemcell(to_bool() == other.to_bool());    
        }

        bool
        UndefMemcell::to_bool() const {
            return false;   
        }
        //--------------UndefMemcell--------------//

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
        } //namespace
    } //namespace memcell
} //namespace avm
