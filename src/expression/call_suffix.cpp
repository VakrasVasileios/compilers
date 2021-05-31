#include "call_suffix.h"

namespace expression {
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

    void 
    CallSuffix::InsertArg(Expression* arg) {
        PRECONDITION(arg != nullptr);
        elist->exprs.push_back(arg);
    }
    
    inline bool IsMethodCall(CallSuffix* call_suffix) {
        return call_suffix->get_type() == METHOD_CALL;
    }
}
