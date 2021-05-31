#include "../../include/virtual_machine/program_consts.h"
#include <assert.h>

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
        index = number_array.size();
        number_map.insert( {num, index} );
        number_array.push_back(num);
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
        index = string_array.size();
        string_map.insert( {str, index} );
        string_array.push_back(str);
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
        index = libfunc_array.size();
        libfunc_map.insert( {str, index} );
        libfunc_array.push_back(str);
    }
    else {
        index = libfunc_map[str];
    }
    return index;
}

double
ProgramConsts:: GetNumber(unsigned index) {
    assert(index < number_array.size());
    return number_array[index];
}

std::string
ProgramConsts:: GetString(unsigned index) {
    assert(index < string_array.size());
    return string_array[index];
}

std::string
ProgramConsts:: GetLibFunc(unsigned index) {
    assert(index < libfunc_array.size());
    return libfunc_array[index];
}