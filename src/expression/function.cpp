#include "../../include/expression/function.h"

std::list<FormalVariable*>
Function:: get_formal_arguments() const {
    return formal_arguments;
}