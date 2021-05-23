#include "call_suffix.h"

CallSuffixType 
CallSuffix:: get_type() const {
    return type;
}

std::string 
CallSuffix:: get_name() const {
    return name;
}

Elist*
CallSuffix:: get_elist() const {
    return elist;
}
