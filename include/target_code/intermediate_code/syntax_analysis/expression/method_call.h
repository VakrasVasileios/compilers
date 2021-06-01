#ifndef METHOD_CALL_H
#define METHOD_CALL_H

#include "call_suffix.h"

namespace expression {
    /**
     * @brief A method callsuffix.
     * 
     */
    class MethodCall final : public CallSuffix {
    public:
        /**
         * @brief Constructs a new MethodCall object.
         * 
         * @param name 
         * @param elist 
         */
        MethodCall(std::string name, Elist* elist)
        : CallSuffix(METHOD_CALL, name, elist) {};
        /**
         * @brief Destroys this MethodCall object.
         * 
         */
        ~MethodCall() = default;
    };
}

#endif 
