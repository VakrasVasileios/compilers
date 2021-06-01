#ifndef CALL_H
#define CALL_H

#include "primary.h"
#include "symbol.h"
#include <list>
#include "../../../../../util/contract/contract.h"
#include "call_suffix.h"

namespace expression {
    /**
     * @brief A call primary expression.
    **/ 
    class Call final : public Primary {
    public:
        /**
         * @brief Constructs a new Call object.
         * 
         * @param called_symbol the called symbol of this Call, not null
         * @param return_val the return value of this call, not null
         */
        Call(Symbol* called_symbol, CallSuffix* call_suffix, Symbol* return_val)
        : Primary(CALL), called_symbol(verify_called_symbol(called_symbol)),
        call_suffix(verify_call_suffix(call_suffix)), ret_val(verify_ret_val(return_val)) {};
        /**
         * @brief Destroys this Call object.
         * 
         */
        ~Call() = default;
        /**
         * @brief Returns a read access to this Call called symbol.
         * 
         * @return a read access to this Call called symbol, can be null
        **/ 
        Symbol*                get_called_symbol() const;
        /**
         * @brief Returns a read access to this Call call suffix. 
         * 
         * @return a read access to this Call call suffix, not null 
         */
        CallSuffix*             get_call_suffix() const;
        /**
         * @brief Returns a read access to this Call return value.
         * 
         * @return a read access to this Call return value, can be null
        **/ 
        Symbol*                 get_ret_val() const;
        
        std::string             to_string() const override;
    protected:
        std::ostream&           LogExpression(std::ostream& os) const override;
    private:
        Symbol*                 called_symbol; 
        CallSuffix*             call_suffix;
        Symbol*                 ret_val;

        Symbol*                 verify_called_symbol(Symbol* called_symbol);
        CallSuffix*             verify_call_suffix(CallSuffix* call_suffix);
        Symbol*                 verify_ret_val(Symbol* ret_val);
    };
}

#endif