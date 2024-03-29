#include <iostream>
#include "../include/avm_memcell.h"
#include "../../../../../util/contains/include/contains.h"

namespace avm 
{
    namespace memcell
    {
        std::ostream& operator << (std::ostream& os, const Table& table) {
            if (table.is_indexed())
                table.log_indexed(os);
            else
                table.log_paired(os);
            return os;        
        }

        bool        
        Table::contains_num_elem(double _key) const {
            return util::contains::has_key<double, AvmMemcell*>(num_map, _key);
        }

        bool        
        Table::contains_str_elem(std::string _key) const {
            return util::contains::has_key<std::string, AvmMemcell*>(str_map,
                _key);
        }

        bool        
        Table::contains_bool_elem(bool _key) const {
            return util::contains::has_key<bool, AvmMemcell*>(bool_map, _key);
        }

        bool        
        Table::contains_tableref_elem(tableref _key) const {
            return util::contains::has_key<tableref, AvmMemcell*>(tableref_map,
                _key);
        }

        bool        
        Table::contains_userfunc_elem(unsigned _key) const {
            return util::contains::has_key<unsigned, AvmMemcell*>(userfunc_map,
                _key);
        }

        bool        
        Table::contains_libfunc_elem(std::string _key) const {
            return util::contains::has_key<std::string, AvmMemcell*>(
                libfunc_map, _key);
        }

        AvmMemcell*
        Table::get_num_elem(double _key) const {
            PRECONDITION(contains_num_elem(_key));
            auto res = num_map.at(_key);
            POSTCONDITION(res != nullptr);
            return res;
        }

        AvmMemcell*
        Table::get_str_elem(std::string _key) const {
            PRECONDITION(contains_str_elem(_key));
            auto res = str_map.at(_key);
            POSTCONDITION(res != nullptr);
            return res;
        }

        AvmMemcell*
        Table::get_bool_elem(bool _key) const {
            PRECONDITION(contains_bool_elem(_key));
            auto res = bool_map.at(_key);
            POSTCONDITION(res != nullptr);
            return res;
        }

        AvmMemcell*
        Table::get_tableref_elem(tableref _key) const {
            PRECONDITION(_key != nullptr);
            PRECONDITION(contains_tableref_elem(_key));
            auto res = tableref_map.at(_key);
            POSTCONDITION(res != nullptr);
            return res;

        }

        AvmMemcell*
        Table::get_userfunc_elem(unsigned _key) const {
            PRECONDITION(contains_userfunc_elem(_key));
            auto res = userfunc_map.at(_key);
            POSTCONDITION(res != nullptr);
            return res;
        }

        AvmMemcell*
        Table::get_libfunc_elem(std::string _key) const {
            PRECONDITION(contains_libfunc_elem(_key));
            auto res = libfunc_map.at(_key);
            POSTCONDITION(res != nullptr);
            return res;
        }

        void        
        Table::set_num_elem(double _key, AvmMemcell* _value) {
            PRECONDITION(_value != nullptr);
            num_map.insert({_key, _value->clone()});
            POSTCONDITION(!num_map.empty());
        }
 
        void
        Table::set_str_elem(std::string _key, AvmMemcell* _value) {
            PRECONDITION(_value != nullptr);
            str_map.insert({_key, _value->clone()});
            POSTCONDITION(!str_map.empty());
        }

        void        
        Table::set_bool_elem(bool _key, AvmMemcell* _value) {
            PRECONDITION(_value != nullptr);
            bool_map.insert({_key, _value->clone()});
            POSTCONDITION(!bool_map.empty());
        }

        void        
        Table::set_tableref_elem(tableref _key, AvmMemcell* _value) {
            PRECONDITION(_value != nullptr);
            PRECONDITION(_key != nullptr);
            tableref_map.insert({_key, _value->clone()});
            POSTCONDITION(!tableref_map.empty());
        }

        void        
        Table::set_userfunc_elem(unsigned _key, AvmMemcell* _value) {
            PRECONDITION(_value != nullptr);
            userfunc_map.insert({_key, _value->clone()});
            POSTCONDITION(!userfunc_map.empty());
        }

        void        
        Table::set_libfunc_elem(std::string _key, AvmMemcell* _value) {
            PRECONDITION(_value != nullptr);
            libfunc_map.insert({_key, _value->clone()});
            POSTCONDITION(!libfunc_map.empty());
        }

        bool
        Table::is_indexed() const {
            return !num_map.empty() & str_map.empty() & bool_map.empty() & 
                tableref_map.empty() & userfunc_map.empty()
                & libfunc_map.empty();
        }

        void
        Table::log_indexed(std::ostream& os) const {
            assert (is_indexed());
            assert (!num_map.empty());
            auto first = true;
            std::map<double, AvmMemcell*>::const_iterator it;
            os << "[ ";
            for (double i = 0; i < num_map.size(); i++) {
                if (!first)
                    os << ", ";
                first = false;
                os << *num_map.at(i);
            }
            os << " ]";
        } 

        void
        Table::log_paired(std::ostream& os) const {
            assert (!is_indexed());
            std::map<AvmMemcell*, AvmMemcell*>::const_iterator it;
            os << "[ ";
            bool first_log = true;
            if (!num_map.empty())
                log_num_map(os, first_log);
            if (!str_map.empty())    
                log_str_map(os, first_log);
            if (!bool_map.empty())
                log_bool_map(os, first_log);
            if (!tableref_map.empty())
                log_table_map(os, first_log);
            if (!userfunc_map.empty())
                log_userfunc_map(os, first_log);
            if (!libfunc_map.empty())
                log_libfunc_map(os, first_log);
            os << " ]";
        }

        namespace
        {
            void out_comma_if_notfirstlog(std::ostream& os, bool &first_log) {
                if (!first_log)
                    os << ", ";
                first_log = false;    
            }
        }

        void    
        Table::log_num_map(std::ostream& os, bool &first_log) const {
            std::map<double, AvmMemcell*>::const_iterator it;
            out_comma_if_notfirstlog(os, first_log);
            auto first = true;
            for (it = num_map.begin(); it != num_map.end(); it++) {
                if (!first)
                    os << ", ";
                first = false;  
                os << "{ " << it->first << " : " << *it->second << " }";
            }
        }

        void    
        Table::log_str_map(std::ostream& os, bool &first_log) const {
            std::map<std::string, AvmMemcell*>::const_iterator it;
            out_comma_if_notfirstlog(os, first_log);
            auto first = true;
            for (it = str_map.begin(); it != str_map.end(); it++) {
                if (!first)
                    os << ", ";
                first = false;  
                os << "{ " << it->first << " : " << *it->second << " }";
            }
        }

        void    
        Table::log_bool_map(std::ostream& os, bool &first_log) const {
            std::map<bool, AvmMemcell*>::const_iterator it;
            out_comma_if_notfirstlog(os, first_log);
            auto first = true;
            for (it = bool_map.begin(); it != bool_map.end(); it++) {
                if (!first)
                    os << ", ";
                first = false;  
                os << "{ " << it->first << " : " << *it->second << " }";
            }
        }

        void    
        Table::log_table_map(std::ostream& os, bool &first_log) const {
            std::map<tableref, AvmMemcell*>::const_iterator it;
            out_comma_if_notfirstlog(os, first_log);
            auto first = true;
            for (it = tableref_map.begin(); it != tableref_map.end(); it++) {
                if (!first)
                    os << ", ";
                first = false;  
                os << "{ " << *it->first << " : " << *it->second << " }";
            }
        }

        void    
        Table::log_userfunc_map(std::ostream& os, bool &first_log) const {
            std::map<unsigned, AvmMemcell*>::const_iterator it;
            out_comma_if_notfirstlog(os, first_log);
            auto first = true;
            for (it = userfunc_map.begin(); it != userfunc_map.end(); it++) {
                if (!first)
                    os << ", ";
                first = false;  
                os << "{ " << "user function " << it->first <<
                    " : " << *it->second << " }";
            }
        }

        void    
        Table::log_libfunc_map(std::ostream& os, bool &first_log) const {
            std::map<std::string, AvmMemcell*>::const_iterator it;
            out_comma_if_notfirstlog(os, first_log);
            auto first = true;
            for (it = libfunc_map.begin(); it != libfunc_map.end(); it++) {
                if (!first)
                    os << ", ";
                first = false;  
                os << "{ " << "library function " << it->first << " : " << 
                *it->second << " }";
            }
        }
    }
}