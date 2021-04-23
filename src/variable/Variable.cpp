#include "Variable.h"
#include <string>

type_t
Variable:: getType() const {
    return type;
}

std::string
Variable:: getName() const {
    return name;
}

unsigned int
Variable:: getLine() const {
    return line;
}

unsigned int
Variable:: getScope() const {
    return scope;
}

std::ostream&
operator<<(std::ostream& os, const Variable variable) {
    return os << "[" << variable.getType() << "] (line: " << variable.getLine() << ")" << " (scope " << variable.getScope() << ")";
}
