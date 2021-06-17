#include "../include/avm_memcell.h"
#include "../../signals/include/signals.h"
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
        AvmMemcell::operator==(AvmMemcell const& other) {
            return equals(other);
        }

        AvmMemcell*
        AvmMemcell::operator!=(AvmMemcell const& other) {
            BoolMemcell* ret = (BoolMemcell*)equals(other);
            bool tmp = !ret->bool_val();
            ret->set_bool_val(tmp);
            return ret;
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
                signals::log_error("Invalid operation \'+\'", std::cerr);
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
                signals::log_error("Second operand is not a number", std::cerr);
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
                signals::log_error("Second operand is not a number", std::cerr);
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
                    signals::log_error("Division with zero", std::cerr);
                }
            }
            else {
                signals::log_error("Second operand is not a number", std::cerr);
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
                    signals::log_error("Modulation with zero", std::cerr);
                }
            }
            else {
                signals::log_error("Second operand is not a number", std::cerr);
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
                signals::log_error("Invalid '>' operation with non-numeric rh operand", std::cerr);
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
                signals::log_error("Invalid '>=' operation with non-numeric rh operand", std::cerr);
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
                signals::log_error("Invalid '<' operation with non-numeric rh operand", std::cerr);
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
                signals::log_error("Invalid '<=' operation with non-numeric rh operand", std::cerr);
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
                signals::log_error("Invalid operation \'+\'", std::cerr);
            }
        }

        AvmMemcell*
        StringMemcell::sub(AvmMemcell const& other) const {
            signals::log_error("Invalid '-' with string operand", std::cerr);
        }

        AvmMemcell*
        StringMemcell::mul(AvmMemcell const& other) const {
            signals::log_error("Invalid '*' with string operand", std::cerr);
        }

        AvmMemcell*
        StringMemcell::div(AvmMemcell const& other) const {
            signals::log_error("Invalid '/' with string operand", std::cerr);
        }

        AvmMemcell*
        StringMemcell::mod(AvmMemcell const& other) const {
            signals::log_error("Invalid '%' with string operand", std::cerr);
        }

        AvmMemcell*
        StringMemcell::gt(AvmMemcell const& other) const {
            signals::log_error("Invalid '>' with string operand", std::cerr);
        }

        AvmMemcell*
        StringMemcell::geq(AvmMemcell const& other) const {
            signals::log_error("Invalid '>=' with string operand", std::cerr);
        }

        AvmMemcell*
        StringMemcell::lt(AvmMemcell const& other) const {
            signals::log_error("Invalid '<' with string operand", std::cerr);
        }

        AvmMemcell*
        StringMemcell::leq(AvmMemcell const& other) const {
            signals::log_error("Invalid '<=' with string operand", std::cerr);
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
            signals::log_error("Invalid '+' with bool operand", std::cerr);
        }

        AvmMemcell*
        BoolMemcell::sub(AvmMemcell const& other) const {
            signals::log_error("Invalid '-' with bool operand", std::cerr);
        }

        AvmMemcell*
        BoolMemcell::mul(AvmMemcell const& other) const {
            signals::log_error("Invalid '*' with bool operand", std::cerr);
        }

        AvmMemcell*
        BoolMemcell::div(AvmMemcell const& other) const {
            signals::log_error("Invalid '/' with bool operand", std::cerr);
        }

        AvmMemcell*
        BoolMemcell::mod(AvmMemcell const& other) const {
            signals::log_error("Invalid '%' with bool operand", std::cerr);
        }

        AvmMemcell*
        BoolMemcell::gt(AvmMemcell const& other) const {
            signals::log_error("Invalid '>' with bool operand", std::cerr);
        }

        AvmMemcell*
        BoolMemcell::geq(AvmMemcell const& other) const {
            signals::log_error("Invalid '>=' with bool operand", std::cerr);
        }

        AvmMemcell*
        BoolMemcell::lt(AvmMemcell const& other) const {
            signals::log_error("Invalid '<' with bool operand", std::cerr);
        }

        AvmMemcell*
        BoolMemcell::leq(AvmMemcell const& other) const {
            signals::log_error("Invalid '<=' with bool operand", std::cerr);
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
            signals::log_error("Invalid '+' with table operand", std::cerr);
        }

        AvmMemcell*
        TableMemcell::sub(AvmMemcell const& other) const {
            signals::log_error("Invalid '-' with table operand", std::cerr);
        }

        AvmMemcell*
        TableMemcell::mul(AvmMemcell const& other) const {
            signals::log_error("Invalid '*' with table operand", std::cerr);
        }

        AvmMemcell*
        TableMemcell::div(AvmMemcell const& other) const {
            signals::log_error("Invalid '/' with table operand", std::cerr);
        }

        AvmMemcell*
        TableMemcell::mod(AvmMemcell const& other) const {
            signals::log_error("Invalid '%' with table operand", std::cerr);
        }

        AvmMemcell*
        TableMemcell::gt(AvmMemcell const& other) const {
            signals::log_error("Invalid '>' with table operand", std::cerr);
        }

        AvmMemcell*
        TableMemcell::geq(AvmMemcell const& other) const {
            signals::log_error("Invalid '>=' with table operand", std::cerr);
        }

        AvmMemcell*
        TableMemcell::lt(AvmMemcell const& other) const {
            signals::log_error("Invalid '<' with table operand", std::cerr);
        }

        AvmMemcell*
        TableMemcell::leq(AvmMemcell const& other) const {
            signals::log_error("Invalid '<=' with table operand", std::cerr);
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
            signals::log_error("Invalid '+' with user function operand", std::cerr);
        }

        AvmMemcell*
        UserfuncMemcell::sub(AvmMemcell const& other) const {
            signals::log_error("Invalid '-' with user function operand", std::cerr);
        }

        AvmMemcell*
        UserfuncMemcell::mul(AvmMemcell const& other) const {
            signals::log_error("Invalid '*' with user function operand", std::cerr);
        }

        AvmMemcell*
        UserfuncMemcell::div(AvmMemcell const& other) const {
            signals::log_error("Invalid '/' with user function operand", std::cerr);
        }

        AvmMemcell*
        UserfuncMemcell::mod(AvmMemcell const& other) const {
            signals::log_error("Invalid '%' with user function operand", std::cerr);
        }

        AvmMemcell*
        UserfuncMemcell::gt(AvmMemcell const& other) const {
            signals::log_error("Invalid '>' with user function operand", std::cerr);
        }

        AvmMemcell*
        UserfuncMemcell::geq(AvmMemcell const& other) const {
            signals::log_error("Invalid '>=' with user function operand", std::cerr);
        }

        AvmMemcell*
        UserfuncMemcell::lt(AvmMemcell const& other) const {
            signals::log_error("Invalid '<' with user function operand", std::cerr);
        }

        AvmMemcell*
        UserfuncMemcell::leq(AvmMemcell const& other) const {
            signals::log_error("Invalid '<=' with user function operand", std::cerr);
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
            signals::log_error("Invalid '+' with library function operand", std::cerr);
        }

        AvmMemcell*
        LibfuncMemcell::sub(AvmMemcell const& other) const {
            signals::log_error("Invalid '-' with library function operand", std::cerr);
        }

        AvmMemcell*
        LibfuncMemcell::mul(AvmMemcell const& other) const {
            signals::log_error("Invalid '*' with library function operand", std::cerr);
        }

        AvmMemcell*
        LibfuncMemcell::div(AvmMemcell const& other) const {
            signals::log_error("Invalid '/' with library function operand", std::cerr);
        }

        AvmMemcell*
        LibfuncMemcell::mod(AvmMemcell const& other) const {
            signals::log_error("Invalid '%' with library function operand", std::cerr);
        }

        AvmMemcell*
        LibfuncMemcell::gt(AvmMemcell const& other) const {
            signals::log_error("Invalid '>' with library function operand", std::cerr);
        }

        AvmMemcell*
        LibfuncMemcell::geq(AvmMemcell const& other) const {
            signals::log_error("Invalid '>=' with library function operand", std::cerr);
        }

        AvmMemcell*
        LibfuncMemcell::lt(AvmMemcell const& other) const {
            signals::log_error("Invalid '<' with library function operand", std::cerr);
        }

        AvmMemcell*
        LibfuncMemcell::leq(AvmMemcell const& other) const {
            signals::log_error("Invalid '<=' with library function operand", std::cerr);
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
            signals::log_error("Invalid '+' with nil operand", std::cerr);
        }

        AvmMemcell*
        NilMemcell::sub(AvmMemcell const& other) const {
            signals::log_error("Invalid '-' with nil operand", std::cerr);
        }

        AvmMemcell*
        NilMemcell::mul(AvmMemcell const& other) const {
            signals::log_error("Invalid '*' with nil operand", std::cerr);
        }

        AvmMemcell*
        NilMemcell::div(AvmMemcell const& other) const {
            signals::log_error("Invalid '/' with nil operand", std::cerr);
        }

        AvmMemcell*
        NilMemcell::mod(AvmMemcell const& other) const {
            signals::log_error("Invalid '%' with nil operand", std::cerr);
        }

        AvmMemcell*
        NilMemcell::gt(AvmMemcell const& other) const {
            signals::log_error("Invalid '>' with nil operand", std::cerr);
        }

        AvmMemcell*
        NilMemcell::geq(AvmMemcell const& other) const {
            signals::log_error("Invalid '>=' with nil operand", std::cerr);
        }

        AvmMemcell*
        NilMemcell::lt(AvmMemcell const& other) const {
            signals::log_error("Invalid '<' with nil operand", std::cerr);
        }

        AvmMemcell*
        NilMemcell::leq(AvmMemcell const& other) const {
            signals::log_error("Invalid '<=' with nil operand", std::cerr);
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
