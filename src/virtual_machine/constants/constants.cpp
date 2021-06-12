#include "../../../include/virtual_machine/constants/constants.h"
#include <iostream>

namespace constants{

auto
Constants:: GetInstance() -> Constants& {
    static Constants instance;
    return instance;
}

void
Constants:: InsertNumber(double num) {
    number_array.push_back(num);
}

void
Constants:: InsertString(std::string str) {
    string_array.push_back(str);
}

void
Constants:: InsertLibfunc(std::string str) {
    libfunc_array.push_back(str);
}

void
Constants:: InsertUserfunc(target_code::Userfunc_entry entry) {
    userfunc_array.push_back(entry);
}

double
Constants:: GetNumber(unsigned index) {
    return number_array[index];
}

std::string
Constants:: GetString(unsigned index) {
    return string_array[index];
}

std::string
Constants:: GetLibfunc(unsigned index) {
    return libfunc_array[index];
}

target_code::Userfunc_entry
Constants:: GetUserfunc(unsigned index) {
    return userfunc_array[index];
}

void
Constants:: LogArrays() {
    std::cout << "--------string array--------" << std::endl;
    for(int i = 0; i < string_array.size(); i++) {
        std::cout << "[" << i << "]\t" << string_array[i] << std::endl;
    }

    std::cout << "--------number array--------" << std::endl;
    for(int i = 0; i < number_array.size(); i++) {
        std::cout << "[" << i << "]\t" << number_array[i] << std::endl;
    }

    std::cout << "--------libfunc array--------" << std::endl;
    for(int i = 0; i < libfunc_array.size(); i++) {
        std::cout << "[" << i << "]\t" << libfunc_array[i] << std::endl;
    }

    std::cout << "--------userfunc array--------" << std::endl;
    for(int i = 0; i < userfunc_array.size(); i++) {
        std::cout   << "[" << i << "]\t" 
                    << userfunc_array[i].taddress
                    << userfunc_array[i].local_count
                    << userfunc_array[i].total_args
                    << userfunc_array[i].id << std::endl;
    }
}

}