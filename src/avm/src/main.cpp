#include <iostream>
#include "registers/include/registers.h"
#include "binary_parser/include/binary_parser.h"
#include "cpu/include/cpu.h"

namespace 
{
void  check_argc(int argc) {
    if (argc < 2) {
        perror("Argument list empty");
        exit(EXIT_FAILURE);
    }
}
}

#ifndef TESTING
int main(int argc, char const *argv[])
{
    check_argc(argc);
    avm::registers::initialize();
    avm::binary_parser::parse_binary(argv[1]);
    avm::cpu::run();

    return 0;
}
#endif