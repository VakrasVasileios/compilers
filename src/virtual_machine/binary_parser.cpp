#include "../../include/virtual_machine/binary_parser.h"
#include <fstream>
#include <iostream>

namespace virtual_machine 
{
void parse_binary(const char* _file) {
    PRECONDITION(_file != nullptr);
    std::fstream executable;
    executable.open(_file, std::ios::in | std::ios::binary);
    unsigned int size;

    auto read_string = [size, &executable]() -> std::string {
        std::string str;
        char c;
        do {
            executable >> c;
            str += c;
        } while (c != '\0');
        return str;
    };
    auto read_unsigned = [&executable]() -> unsigned int {
        unsigned int unint;
        executable.read((char*)&unint, sizeof(unsigned int));
        return unint;
    };
    auto read_byte = [&executable]() -> uint8_t {
        uint8_t byte;
        executable.read((char*)&byte, sizeof(uint8_t));
        return byte;
    };

    // read and check if the magic number is right
    unsigned int magic_num = read_unsigned();
    if (magic_num != 340200501) {
        std::cerr << "Not an alpha language executable" << std::endl;
        exit(EXIT_FAILURE);
    }
    // read string array
    size = read_unsigned();
    for (int i = 0; i < size; i++) {
        read_string();
    }
    // read number array
    size = read_unsigned();
    for (int i = 0; i < size; i++) {
        double num;
        executable.read((char*)&num, sizeof(double));
    }
    // read lib function IDs
    size = read_unsigned();
    for (int i = 0; i < size; i++) {
        read_string();
    }
    // read user function
    size = read_unsigned();
    for (int i = 0; i < size; i++) {
        read_unsigned();
        read_unsigned();
        read_unsigned();
        read_string();      
    }
    // read instructions
    size = read_unsigned();
    for (int i = 0; i < size; i++) {
        auto arg_count = read_unsigned();
        auto label = read_unsigned();
        auto opcode = read_byte();
        auto result_opcode = read_byte();
        auto result_value = read_unsigned();

        auto result = target_code::vmarg_factory::create(
            static_cast<target_code::Vmarg_t>(result_opcode), result_value);
        
        target_code::Vmarg* args[2];    
        for (int j = 0; j < arg_count; j++) {
            auto arg_opcode = read_byte();
            auto arg_value = read_unsigned();

            auto arg = target_code::vmarg_factory::create(
                static_cast<target_code::Vmarg_t>(arg_opcode), arg_value);
            args[j] = arg;
        }
        auto src_line = read_unsigned();

        auto instr = target_code::instruction_factory::create(
            static_cast<target_code::Vmopcode>(opcode), label, result,
            args[0], args[1], src_line);
    }
    executable.close();
}
}