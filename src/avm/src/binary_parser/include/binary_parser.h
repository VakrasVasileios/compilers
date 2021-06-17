#ifndef BINARY_PARSER_H
#define BINARY_PARSER_H

#include "../../../../../util/contract/include/contract.h"
#include "../../../../target_code/include/instruction.h"
#include "../../../../target_code/include/vmarg_factory.h"
#include "../../../../target_code/include/instruction_factory.h"
#include "../../memory/include/memory.h"


namespace avm 
{
    namespace binary_parser
    {
        extern unsigned int global_offset;
        /**
         * @brief Loads the target code instructions to the code
         * memory segment and stores the used constants.
         * 
         * @param _file the binary file to parse,
         * not null
         */
        void parse_binary(const char* _file);
    }
}

#endif