#include "../include/avm_memcell.h"
#include "../../signals/include/signals.h"
#include <iostream>
#include <math.h>

namespace avm 
{
    namespace memcell
    {
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
        
        //--------------AvmMemcell--------------//
        std::ostream& operator << (std::ostream& os,
                const AvmMemcell& memcell) {
            return memcell.log(os);    
        }

        AvmMemcell*
        AvmMemcell::operator==(AvmMemcell const& other) const {
            return equals(other);
        }

        AvmMemcell*
        AvmMemcell::operator!=(AvmMemcell const& other) const {
            BoolMemcell* ret = (BoolMemcell*)equals(other);
            if (ret == nullptr)
                return ret;
            else {
                bool tmp = !ret->bool_val();
                ret->set_bool_val(tmp);
                return ret;
            }    
        }

        AvmMemcell*
        AvmMemcell::operator+(AvmMemcell const& other) const {
            return add(other);
        }

        AvmMemcell*
        AvmMemcell::operator-(AvmMemcell const& other) const {
            return sub(other);
        }

        AvmMemcell*
        AvmMemcell::operator*(AvmMemcell const& other) const {
            return mul(other);
        }

        AvmMemcell*
        AvmMemcell::operator/(AvmMemcell const& other) const {
            return div(other);
        }

        AvmMemcell*
        AvmMemcell::operator%(AvmMemcell const& other) const {
            return mod(other);
        }

        AvmMemcell*
        AvmMemcell::operator>(AvmMemcell const& other) const {
            return gt(other);
        }

        AvmMemcell*
        AvmMemcell::operator>=(AvmMemcell const& other) const {
            return geq(other);
        }

        AvmMemcell*
        AvmMemcell::operator<(AvmMemcell const& other) const {
            return lt(other);
        }

        AvmMemcell*
        AvmMemcell::operator<=(AvmMemcell const& other) const {
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

        AvmMemcell* 
        NumMemcell::clone() const {
            return new NumMemcell(*this);    
        }

        std::string  
        NumMemcell::get_type() const {
            return "number";
        }

        bool
        NumMemcell::to_bool() const {
            return num_val_ != 0;    
        }

        std::ostream&   
        NumMemcell::log(std::ostream& os) const {
            return os << num_val_;    
        }

        namespace
        {
            BoolMemcell* equals_heteroids(AvmMemcell const& _lhs,
                AvmMemcell const& _rhs) {
                if (auto nil_memcell = nill_memcell_cast(_rhs)) {
                    return new BoolMemcell(false);
                }
                else if (auto bool_memcell = bool_memcell_cast(_rhs)) {
                    return new BoolMemcell(_lhs.to_bool() ==
                        bool_memcell->bool_val());
                }
                else {
                    signals::log_error(
                        "invalid operation with non matching types: " 
                            + _lhs.get_type() + " and " + _rhs.get_type(),
                                std::cerr);
                    return nullptr; 
                }
            }   
        }

        AvmMemcell*    
        NumMemcell::equals(AvmMemcell const& other) const {
            if (auto num_memcell = num_memcell_cast(other))
                return new BoolMemcell(num_val_ == num_memcell->num_val());
            else    
                return equals_heteroids(*this, other);       
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
                ret->set_str_val(tmp + str->str_val());
                return ret;
            }
            else {
                signals::log_error("Invalid operation \'+\'", std::cerr);
                return nullptr;
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
                return nullptr;
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
                return nullptr;
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
                    return nullptr;
                }
            }
            else {
                signals::log_error("Second operand is not a number", std::cerr);
                return nullptr;
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
                    return nullptr;
                }
            }
            else {
                signals::log_error("Second operand is not a number", std::cerr);
                return nullptr;
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
                return nullptr;
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
                return nullptr;
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
                return nullptr;
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
                return nullptr;
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

        AvmMemcell* 
        StringMemcell::clone() const {
            return new StringMemcell(*this);    
        }

        bool
        StringMemcell::to_bool() const {
            return !str_val_.empty();    
        }

        std::string  
        StringMemcell::get_type() const {
            return "string";    
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
                return equals_heteroids(*this, other);
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
                tmp += std::to_string(num->num_val());
                ret->str_val_ += tmp;
                return ret;
            }
            else {
                signals::log_error("Invalid operation \'+\'", std::cerr);
                return nullptr;
            }
        }

        AvmMemcell*
        StringMemcell::sub(AvmMemcell const& other) const {
            signals::log_error("Invalid '-' with string operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        StringMemcell::mul(AvmMemcell const& other) const {
            signals::log_error("Invalid '*' with string operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        StringMemcell::div(AvmMemcell const& other) const {
            signals::log_error("Invalid '/' with string operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        StringMemcell::mod(AvmMemcell const& other) const {
            signals::log_error("Invalid '%' with string operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        StringMemcell::gt(AvmMemcell const& other) const {
            signals::log_error("Invalid '>' with string operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        StringMemcell::geq(AvmMemcell const& other) const {
            signals::log_error("Invalid '>=' with string operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        StringMemcell::lt(AvmMemcell const& other) const {
            signals::log_error("Invalid '<' with string operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        StringMemcell::leq(AvmMemcell const& other) const {
            signals::log_error("Invalid '<=' with string operand", std::cerr);
            return nullptr;
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

        AvmMemcell* 
        BoolMemcell::clone() const {
            return new BoolMemcell(*this);    
        }

        bool
        BoolMemcell::to_bool() const {
            return bool_val_;   
        }

        std::string  
        BoolMemcell::get_type() const {
            return "boolean";    
        }

        std::ostream&   
        BoolMemcell::log(std::ostream& os) const {
            if (bool_val_)
                return os << "true";
            else
                return os << "false";   
        }

        AvmMemcell*    
        BoolMemcell::equals(AvmMemcell const& other) const {
            if (auto undef_memcell = undef_memcell_cast(other)) {
                signals::log_error("invalid comparison with undefined type",
                    std::cerr);
                return nullptr;
            }
            else
            if (auto nil_memcell = nill_memcell_cast(other)) {
                return new BoolMemcell(false);
            }
            else
                return new BoolMemcell(bool_val_ == other.to_bool());
        }

        AvmMemcell*
        BoolMemcell::add(AvmMemcell const& other) const {
            signals::log_error("Invalid '+' with bool operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        BoolMemcell::sub(AvmMemcell const& other) const {
            signals::log_error("Invalid '-' with bool operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        BoolMemcell::mul(AvmMemcell const& other) const {
            signals::log_error("Invalid '*' with bool operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        BoolMemcell::div(AvmMemcell const& other) const {
            signals::log_error("Invalid '/' with bool operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        BoolMemcell::mod(AvmMemcell const& other) const {
            signals::log_error("Invalid '%' with bool operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        BoolMemcell::gt(AvmMemcell const& other) const {
            signals::log_error("Invalid '>' with bool operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        BoolMemcell::geq(AvmMemcell const& other) const {
            signals::log_error("Invalid '>=' with bool operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        BoolMemcell::lt(AvmMemcell const& other) const {
            signals::log_error("Invalid '<' with bool operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        BoolMemcell::leq(AvmMemcell const& other) const {
            signals::log_error("Invalid '<=' with bool operand", std::cerr);
            return nullptr;
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
        TableMemcell::clone() const {
            return new TableMemcell(*this);    
        }

        AvmMemcell*        
        TableMemcell::equals(AvmMemcell const& other) const {
            if (auto table_memcell = table_memcell_cast(other)) 
                return new BoolMemcell(true); // TOCHANGE
                // return table_val_ == table_memcell->table_val();
            else    
                return equals_heteroids(*this, other);
            // else
            // if (auto table_memcell = table_memcell_cast(other))
            //     // return table_val_ == table_memcell->table_val(); TODO: check maps
            // // else    
            // //     return to_bool() == other.to_bool(); 
            // return new BoolMemcell(to_bool() == other.to_bool());
        }

        bool
        TableMemcell::to_bool() const {
            return true;   
        }

        std::string         
        TableMemcell::get_type() const {
            return "table";
        }

        std::ostream&   
        TableMemcell::log(std::ostream& os) const {
            return os; //TODO    
        }

        AvmMemcell*
        TableMemcell::add(AvmMemcell const& other) const {
            signals::log_error("Invalid '+' with table operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        TableMemcell::sub(AvmMemcell const& other) const {
            signals::log_error("Invalid '-' with table operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        TableMemcell::mul(AvmMemcell const& other) const {
            signals::log_error("Invalid '*' with table operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        TableMemcell::div(AvmMemcell const& other) const {
            signals::log_error("Invalid '/' with table operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        TableMemcell::mod(AvmMemcell const& other) const {
            signals::log_error("Invalid '%' with table operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        TableMemcell::gt(AvmMemcell const& other) const {
            signals::log_error("Invalid '>' with table operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        TableMemcell::geq(AvmMemcell const& other) const {
            signals::log_error("Invalid '>=' with table operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        TableMemcell::lt(AvmMemcell const& other) const {
            signals::log_error("Invalid '<' with table operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        TableMemcell::leq(AvmMemcell const& other) const {
            signals::log_error("Invalid '<=' with table operand", std::cerr);
            return nullptr;
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
        UserfuncMemcell::clone() const {
            return new UserfuncMemcell(*this);    
        }

        AvmMemcell*            
        UserfuncMemcell::equals(AvmMemcell const& other) const {
            if (auto userfunc_memcell = userfunc_memcell_cast(other))
                return new BoolMemcell(func_val_ == userfunc_memcell->func_val());
            else    
                return equals_heteroids(*this, other);
        }

        bool
        UserfuncMemcell::to_bool() const {
            return true;   
        }

        std::string     
        UserfuncMemcell::get_type() const {
            return "userfunction";    
        }

        std::ostream&   
        UserfuncMemcell::log(std::ostream& os) const {
            return os << "user function"; 
        }

        AvmMemcell*
        UserfuncMemcell::add(AvmMemcell const& other) const {
            signals::log_error("Invalid '+' with user function operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        UserfuncMemcell::sub(AvmMemcell const& other) const {
            signals::log_error("Invalid '-' with user function operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        UserfuncMemcell::mul(AvmMemcell const& other) const {
            signals::log_error("Invalid '*' with user function operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        UserfuncMemcell::div(AvmMemcell const& other) const {
            signals::log_error("Invalid '/' with user function operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        UserfuncMemcell::mod(AvmMemcell const& other) const {
            signals::log_error("Invalid '%' with user function operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        UserfuncMemcell::gt(AvmMemcell const& other) const {
            signals::log_error("Invalid '>' with user function operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        UserfuncMemcell::geq(AvmMemcell const& other) const {
            signals::log_error("Invalid '>=' with user function operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        UserfuncMemcell::lt(AvmMemcell const& other) const {
            signals::log_error("Invalid '<' with user function operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        UserfuncMemcell::leq(AvmMemcell const& other) const {
            signals::log_error("Invalid '<=' with user function operand", std::cerr);
            return nullptr;
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
        LibfuncMemcell::clone() const {
            return new LibfuncMemcell(*this);      
        }

        AvmMemcell*        
        LibfuncMemcell::equals(AvmMemcell const& other) const {
            if (auto libfunc_memcell = libfunc_memcell_cast(other))
                return new BoolMemcell(lib_func_val_ == libfunc_memcell->lib_func_val());
            else    
                return equals_heteroids(*this, other);
        }

        bool
        LibfuncMemcell::to_bool() const {
            return true;   
        }

        std::string  
        LibfuncMemcell::get_type() const {
            return "libraryfunction";    
        }

        std::ostream&   
        LibfuncMemcell::log(std::ostream& os) const {
            return os << "library function";  
        }

        AvmMemcell*
        LibfuncMemcell::add(AvmMemcell const& other) const {
            signals::log_error("Invalid '+' with library function operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        LibfuncMemcell::sub(AvmMemcell const& other) const {
            signals::log_error("Invalid '-' with library function operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        LibfuncMemcell::mul(AvmMemcell const& other) const {
            signals::log_error("Invalid '*' with library function operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        LibfuncMemcell::div(AvmMemcell const& other) const {
            signals::log_error("Invalid '/' with library function operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        LibfuncMemcell::mod(AvmMemcell const& other) const {
            signals::log_error("Invalid '%' with library function operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        LibfuncMemcell::gt(AvmMemcell const& other) const {
            signals::log_error("Invalid '>' with library function operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        LibfuncMemcell::geq(AvmMemcell const& other) const {
            signals::log_error("Invalid '>=' with library function operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        LibfuncMemcell::lt(AvmMemcell const& other) const {
            signals::log_error("Invalid '<' with library function operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        LibfuncMemcell::leq(AvmMemcell const& other) const {
            signals::log_error("Invalid '<=' with library function operand", std::cerr);
            return nullptr;
        }
        //--------------LibfuncMemcell--------------//

        //--------------NilMemcell--------------//
        void
        NilMemcell::accept(AvmMemcellVisitor* visitor) {
            PRECONDITION(visitor != nullptr);
            visitor->visit_nill_memcell(this);
        }

        AvmMemcell* 
        NilMemcell::clone() const {
            return new NilMemcell(*this);      
        }

        AvmMemcell*
        NilMemcell::equals(AvmMemcell const& other) const {
            if (auto undef_memcell = undef_memcell_cast(other)) {
                signals::log_error("invalid comparison with undefined type",
                    std::cerr);
                return nullptr;
            }
            else if (auto nil_memcell = nill_memcell_cast(other))
                return new BoolMemcell(true);
            else
                return new BoolMemcell(false);           
        }

        bool
        NilMemcell::to_bool() const {
            return false;   
        }

        std::string  
        NilMemcell::get_type() const {
            return "nil";    
        }

        std::ostream& 
        NilMemcell::log(std::ostream& os) const {
            return os << "nil";    
        }

        AvmMemcell*
        NilMemcell::add(AvmMemcell const& other) const {
            signals::log_error("Invalid '+' with nil operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        NilMemcell::sub(AvmMemcell const& other) const {
            signals::log_error("Invalid '-' with nil operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        NilMemcell::mul(AvmMemcell const& other) const {
            signals::log_error("Invalid '*' with nil operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        NilMemcell::div(AvmMemcell const& other) const {
            signals::log_error("Invalid '/' with nil operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        NilMemcell::mod(AvmMemcell const& other) const {
            signals::log_error("Invalid '%' with nil operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        NilMemcell::gt(AvmMemcell const& other) const {
            signals::log_error("Invalid '>' with nil operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        NilMemcell::geq(AvmMemcell const& other) const {
            signals::log_error("Invalid '>=' with nil operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        NilMemcell::lt(AvmMemcell const& other) const {
            signals::log_error("Invalid '<' with nil operand", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        NilMemcell::leq(AvmMemcell const& other) const {
            signals::log_error("Invalid '<=' with nil operand", std::cerr);
            return nullptr;
        }
        //--------------NilMemcell--------------//
        //--------------UndefMemcell--------------//
        void
        UndefMemcell::accept(AvmMemcellVisitor* visitor) {
            PRECONDITION(visitor != nullptr);
            visitor->visit_undef_memcell(this);
        }

        AvmMemcell* 
        UndefMemcell::clone() const {
            return new UndefMemcell(*this);      
        }

        bool
        UndefMemcell::to_bool() const {
            return false;   
        }

        std::string
        UndefMemcell::get_type() const {
            return "undefined";
        }

        std::ostream&
        UndefMemcell::log(std::ostream& os) const {
            signals::log_error("cannot output undefined type", std::cerr);
            return os;    
        }

        AvmMemcell*
        UndefMemcell::equals(AvmMemcell const& other) const {
            signals::log_error("illegal comparison with undefined type",
                std::cerr);
            return nullptr;    
        }

        AvmMemcell*
        UndefMemcell::add(AvmMemcell const& other) const {
            signals::log_error(
                "illegal arithmetic operation with undefined type", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        UndefMemcell::sub(AvmMemcell const& other) const {
             signals::log_error(
                "illegal arithmetic operation with undefined type", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        UndefMemcell::mul(AvmMemcell const& other) const {
             signals::log_error(
                "illegal arithmetic operation with undefined type", std::cerr);
            return nullptr;
        }

        AvmMemcell*
        UndefMemcell::div(AvmMemcell const& other) const {
             signals::log_error(
                "illegal arithmetic operation with undefined type", std::cerr);
            return nullptr;
        }
        
        AvmMemcell*
        UndefMemcell::mod(AvmMemcell const& other) const {
             signals::log_error(
                "illegal arithmetic operation with undefined type", std::cerr);
            return nullptr;
        }
        
        AvmMemcell*
        UndefMemcell::gt(AvmMemcell const& other) const {
            signals::log_error("illegal comparison with undefined type",
                std::cerr);
            return nullptr;
        }

        AvmMemcell*
        UndefMemcell::geq(AvmMemcell const& other) const {
            signals::log_error("illegal comparison with undefined type",
                std::cerr);
            return nullptr;
        }

        AvmMemcell*
        UndefMemcell::lt(AvmMemcell const& other) const {
            signals::log_error("illegal comparison with undefined type",
                std::cerr);
            return nullptr;
        }

        AvmMemcell*
        UndefMemcell::leq(AvmMemcell const& other) const {
            signals::log_error("illegal comparison with undefined type",
                std::cerr);
            return nullptr;
        }
        //--------------UndefMemcell--------------//

    } //namespace memcell
} //namespace avm
