#include "../../include/virtual_machine/binary_parser.h"
#include "../../include/virtual_machine/constants/constants.h"
#include <iostream>


int main(int argc, char const *argv[])
{
    if (argc < 2) {
        perror("Argument list empty");
        exit(EXIT_FAILURE);
    }
    virtual_machine::parse_binary(argv[1]);
    constants::Constants::GetInstance().LogArrays();

    return 0;
}