#include "../include/Variable.h"
#include <assert.h>
#include <string>

std::ostream&
operator<<(std::ostream& os, const Variable &v) {
    switch(v.type) {
        case LIB_FUNC:
            return  os << "[library function] (line: " << v.line << ")" << "  (scope " << v.scope << ")";
        case USER_FUNC:
            return os << "[user function] (line: " << v.line << ")" << "  (scope " << v.scope << ")";
        case LOCAL_VAR:
            return os << "[local variable] (line: " << v.line << ")" << "  (scope " << v.scope << ")";
        case GLOBAL_VAR:
            return os << "[global variable] (line: " << v.line << ")" << "  (scope " << v.scope << ")";
        case FORMAL_ARG:
            return os << "[formal argument] (line: " << v.line << ")" << "  (scope " << v.scope << ")";
        default:
            assert (false);
    }
}
