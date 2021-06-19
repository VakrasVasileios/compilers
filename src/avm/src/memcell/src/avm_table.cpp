#include "../include/avm_memcell.h"
#include "../../../../../util/contains/include/contains.h"

namespace avm 
{
    namespace memcell
    {
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
    }
}