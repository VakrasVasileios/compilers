#ifndef NORM_CALL_H
#define NORM_CALL_H

#include "call_suffix.h"

namespace expression {
    /**
     * @brief A normal callsuffix.
     * 
     */
    class NormCall final : public CallSuffix {
    public:
        /**
         * @brief Constructs a new NormCall object.
         * 
         * @param elist 
         */
        NormCall(Elist* elist)
        : CallSuffix(NORM_CALL, std::string(""), elist) {};
        /**
         * @brief Destroys this NormCall object.
         * 
         */
        ~NormCall() = default;
    };
}

#endif 
