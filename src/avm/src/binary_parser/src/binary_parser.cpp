#include "../include/binary_parser.h"

#include <fstream>
#include <iostream>

namespace avm 
{
    namespace binary_parser
    {
    using namespace memory;

    unsigned int global_offset;
    void parse_binary(const char* _file) {
        PRECONDITION(_file != nullptr);
        std::fstream executable;
        executable.open(_file, std::ios::in | std::ios::binary);
        unsigned int size;

        auto read_string = [&executable]() -> std::string {
            std::string str;
            char c;
            do {
                executable.read((char*)&c, sizeof(char));
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
        auto read_double = [&executable]() -> double {
            double num;
            executable.read((char*)&num, sizeof(double));
            return num;
        };

        // read and check if the magic number is right
        unsigned int magic_num = read_unsigned();
        if (magic_num != 340200501) {
            std::cerr << "Not an alpha language executable" << std::endl;
            exit(EXIT_FAILURE);
        }

        global_offset = read_unsigned();
        // read string array
        size = read_unsigned();
        for (int i = 0; i < size; i++) {
            Constants:: GetInstance().InsertString(read_string());
        }
        // read number array
        size = read_unsigned();
        for (int i = 0; i < size; i++) {
            Constants:: GetInstance().InsertNumber(read_double());
        }
        // read lib function IDs
        size = read_unsigned();
        for (int i = 0; i < size; i++) {
            Constants:: GetInstance().InsertLibfunc(read_string());
        }
        // read user function
        size = read_unsigned();
        for (int i = 0; i < size; i++) {
            unsigned taddr = read_unsigned();
            unsigned totloc = read_unsigned();
            unsigned totargs = read_unsigned();
            std::string id = read_string();
            target_code::Userfunc_entry entry(taddr, totloc, totargs, id);
            Constants:: GetInstance().InsertUserfunc(entry);
        }

        // std::cout << "CONSTANTS:\n" << Constants:: GetInstance() << std::endl;

        // read instructions
        size = read_unsigned();
        for (int i = 0; i < size; i++) {
            auto arg_count = read_unsigned();
            auto label = read_unsigned();
            auto opcode = read_byte();
            auto result_opcode = read_byte();
            auto result_value = read_unsigned();

            auto result = target_code::create_vmarg(
                static_cast<target_code::Vmarg_t>(result_opcode), result_value);
            
            target_code::Vmarg* args[2];    
            for (int j = 0; j < arg_count; j++) {
                auto arg_opcode = read_byte();
                auto arg_value = read_unsigned();

                auto arg = target_code::create_vmarg(
                    static_cast<target_code::Vmarg_t>(arg_opcode), arg_value);
                args[j] = arg;
            }
            auto src_line = read_unsigned();

            auto instr = target_code::create_instruction(
                static_cast<target_code::Vmopcode>(opcode), label, result,
                args[0], args[1], src_line);
                
            code_segment.push(instr);    
        }
        // end of program flag
        code_segment.push(new target_code::Jump(memory::code_segment.size() + 1, new target_code::LabelVmarg(0), 0));
        executable.close();
        // for (unsigned i = 0; i < memory::code_segment.size(); i++) {
        //     std::cout << memory::code_segment[i] << std::endl;
        // }
    }
    } // namespace binary_parser
} // namespace avm