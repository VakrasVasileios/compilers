#ifndef AVM_CPU_H
#define AVM_CPU_H

#include "../../memcell/include/avm_memcell.h"
#include "../../stack_segment/include/avm_stack_segment.h"

namespace avm 
{
class Cpu final {
public:
    /**
     * @brief Constructs a new Cpu object.
     * 
     */
    Cpu() : pc_(0) {}
    /**
     * @brief Destroys this Cpu object.
     * 
     */
    ~Cpu() = default;    
private:
    unsigned pc_;
    memcell::AvmMemcell *ax_, *bx_, *cx_;
    memcell::AvmMemcell *retval_;
};
}


#endif