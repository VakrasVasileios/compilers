#ifndef BINARY_PARSER_H
#define BINARY_PARSER_H

#include "../../../util/contract/include/contract.h"
#include "../../target_code/include/instruction.h"
#include "../../target_code/include/vmarg_factory.h"
#include "../../target_code/include/instruction_factory.h"

namespace avm {
    void parse_binary(const char* _file);
}

#endif