#include "binary_parser.h"
#include "constants/include/constants.h"
#include <iostream>

#ifndef TESTING
int main(int argc, char const *argv[])
{
    if (argc < 2) {
        perror("Argument list empty");
        exit(EXIT_FAILURE);
    }
    avm::parse_binary(argv[1]);

    return 0;
}
#endif