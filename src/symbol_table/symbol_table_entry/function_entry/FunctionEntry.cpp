#include "FunctionEntry.h"

std::list<FormalVariableEntry>
FunctionEntry:: getFormalArguments() const {
    return formalArguments;
}

std::ostream&
operator<<(std::ostream& os, const FunctionEntry functionEntry) {
    return os << functionEntry;
}