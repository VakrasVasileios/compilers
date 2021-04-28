#include "../../include/id/Id.h"
#include <string.h>

std::string
Id:: getName() const {
    return name;
}

unsigned int
Id:: getLine() const {
    return line;
}

unsigned int
Id:: getScope() const {
    return scope;
}

std::ostream&
operator<<(std::ostream& os, const Id id) {
    return os << "\"" << id.getName() << "\" (line: " << id.getLine() << ")" << " (scope " << id.getScope() << ")";
}