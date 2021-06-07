#include "../../include/virtual_machine/binary_parser.h"
#include <fstream>
#include <iostream>

namespace virtual_machine {
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
        std::cout << magic_num << std::endl;
        if (magic_num != 340200501) {
            std::cerr << "Not an alpha language executable" << std::endl;
            exit(EXIT_FAILURE);
        }
        // read string array
        size = read_unsigned();
        for (int i = 0; i < size; i++) {
            std::cout << read_string() << std::endl;
        }
        // read number array
        size = read_unsigned();
        for (int i = 0; i < size; i++) {
            double num;
            executable.read((char*)&num, sizeof(double));
            std::cout << num << std::endl;
        }
        // read lib function IDs
        size = read_unsigned();
        for (int i = 0; i < size; i++) {
            std::cout << read_string() << std::endl;
        }
        // read user function
        size = read_unsigned();
        for (int i = 0; i < size; i++) {
            std::cout << "taddress: " << read_unsigned() << std::endl;
            std::cout << "local count: " << read_unsigned() << std::endl;
            std::cout << "total args: " << read_unsigned() << std::endl;
            std::cout << "id: " << read_string() << std::endl;         
        }
        // read instructions
        size = read_unsigned();
        for (int i = 0; i < size; i++) {
            auto arg_count = read_unsigned();
            auto opcode = read_byte();
            printf("args: %d\nopcode: %d\n", arg_count, (int)opcode);
            auto result_opcode = read_byte();
            auto result_value = read_unsigned();
            std::cout << "result opcode: " << result_opcode << " result value: " << result_value << std::endl;
            for (int j = 0; j < arg_count; j++) {
                auto arg_opcode = read_byte();
                auto arg_value = read_unsigned();
                std::cout << "arg opcode: " << arg_opcode << " arg value: " << arg_value << std::endl;
            }
        }


        executable.close();
    }
}