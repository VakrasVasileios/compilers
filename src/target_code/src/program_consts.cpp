#include "program_consts.h"
#include <assert.h>

namespace target_code {
    ProgramConsts:: ProgramConsts() {
        number_array = new std::vector<double>();
        string_array = new std::vector<std::string>();
        libfunc_array = new std::vector<std::string>();
        userfunc_array = new std::vector<Userfunc_entry>();
    }

    ProgramConsts& 
    ProgramConsts:: GetInstance(void) {
        static ProgramConsts singleton;
        return singleton;
    }

    const ProgramConsts&
    ProgramConsts:: GetInstanceConst(void) {
        static const ProgramConsts singleton;
        return singleton;
    }

    unsigned
    ProgramConsts:: InsertNumber(double num) {
        unsigned index;
        if (number_map.find(num) == number_map.end()) {
            index = number_array->size();
            number_map.insert( {num, index} );
            number_array->push_back(num);
        }
        else {
            index = number_map[num];
        }
        return index;
    }

    unsigned
    ProgramConsts:: InsertString(const std::string& str) {
        unsigned index;
        if (string_map.find(str) == string_map.end()) {
            index = string_array->size();
            string_map.insert( {str, index} );
            string_array->push_back(str);
        }
        else {
            index = string_map[str];
        }
        return index;
    }

    unsigned
    ProgramConsts:: InsertLibFunc(const std::string& str) {
        unsigned index;
        if (libfunc_map.find(str) == libfunc_map.end()) {
            index = libfunc_array->size();
            libfunc_map.insert( {str, index} );
            libfunc_array->push_back(str);
        }
        else {
            index = libfunc_map[str];
        }
        return index;
    }

    unsigned
    ProgramConsts:: InsertUserFunc(expression::Symbol* userfunc) {
        unsigned index;
        if (userfunc_map.find(userfunc->get_taddress()) == userfunc_map.end()) {
            index = userfunc_array->size();
            Userfunc_entry entry(userfunc->get_taddress(), userfunc->get_total_local(),
                                userfunc->get_formal_arguments().size(), userfunc->get_id());
            userfunc_map.insert( {entry.taddress, index} );
            userfunc_array->push_back(entry);
        }
        else {
            index = userfunc_map[userfunc->get_taddress()];
        }
        return index;
    }

    double
    ProgramConsts:: GetNumber(unsigned index) {
        assert(index < number_array->size());
        return (*number_array)[index];
    }

    std::string
    ProgramConsts:: GetString(unsigned index) {
        assert(index < string_array->size());
        return (*string_array)[index];
    }

    std::string
    ProgramConsts:: GetLibFunc(unsigned index) {
        assert(index < libfunc_array->size());
        return (*libfunc_array)[index];
    }

    unsigned
    ProgramConsts:: GetUserFunc(unsigned index) {
        assert(index < userfunc_array->size());
        return (*userfunc_array)[index].taddress;
    }

    std::vector<double>*
    ProgramConsts:: GetNumberArray() const {
        return number_array;
    }

    std::vector<std::string>*
    ProgramConsts:: GetStringArray() const {
        return string_array;
    }

    std::ostream &operator<<(std::ostream &os, const ProgramConsts &rhs) {
        os << std::endl << "-----------  number_array  -----------" << std::endl;
        rhs.LogNumberArray(os);
        os << std::endl << "-----------  string_array  -----------" << std::endl;
        rhs.LogStringArray(os);
        os << std::endl << "-----------  libfunc_array  -----------" << std::endl;
        rhs.LogLibFuncArray(os);
        os << std::endl << "-----------  userfunc_array  -----------" << std::endl;
        rhs.LogUserFuncArray(os);
        return os;
    }

    std::vector<std::string>*
    ProgramConsts:: GetLibFuncArray() const {
        return libfunc_array;
    }

    std::vector<Userfunc_entry>*
    ProgramConsts:: GetUserFuncArray() const {
        return userfunc_array;
    }

    void
    ProgramConsts::LogNumberArray(std::ostream &os) const {
        for (int i = 0; i < (*number_array).size(); i++)
            os << "number_array[" << i << "] " << (*number_array)[i] << std::endl;
    }

    void
    ProgramConsts::LogStringArray(std::ostream &os) const {
        for (int i = 0; i < (*string_array).size(); i++)
            os << "string_array[" << i << "] " << (*string_array)[i] << std::endl;
    }

    void
    ProgramConsts::LogLibFuncArray(std::ostream &os) const {
        for (int i = 0; i < (*libfunc_array).size(); i++)
            os << "libfunc_array[" << i << "] " << (*libfunc_array)[i] << std::endl;
    }

    void
    ProgramConsts::LogUserFuncArray(std::ostream &os) const {
        for (int i = 0; i < (*userfunc_array).size(); i++)
            os << "userfunc_array[" << i << "] " << (*userfunc_array)[i].id << std::endl;
    }
}