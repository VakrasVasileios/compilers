#ifndef AVM_STACK_SEGMENT_H
#define AVM_STACK_SEGMENT_H

#include <vector>
#include "../../util/contract/contract.h"
#include "../../util/range/range.h"
#include "../target_code/target_code.h"
#include "avm_memcell.h"
#include "avm_cpu.h"

namespace virtual_machine
{
/**
 * @brief The avm stack segment containing the global
 * variables and the call stack section.
 * 
 */
class AvmStackSegment final {
public:
    /**
     * @brief Constructs a new AvmStackSegment object.
     * 
     */
    AvmStackSegment() : _size(0), _top(AVM_STACKSIZE) {}
    /**
     * @brief Destroys this AvmStackSegment object.
     * 
     */
    ~AvmStackSegment() = default;
    /**
     * @brief Returns a read access to this AvmStackSegment
     * size.
     * 
     * @return a read access to this AvmStackSegment
     * size
     */
    int         size() const;
    /**
     * @brief Checks wether this AvmStackSegment is empty.
     * 
     * @return  wether this AvmStackSegment is empty
     */
    bool        empty() const;
    /**                                                    
     * @brief Returns a read/write access to the top
     * avm memcell of this non empty AvmStackSegment.
     * 
     * @return a read/write access to the top
     * avm memcell of non empty this AvmStackSegment 
     */
    AvmMemcell  top() const;
    /**
     * @brief Pushes a memcell at the top of this
     * AvmStackSegment.
     * 
     * @param _memcell the memcell to be pushed
     * at the top of this AvmStackSegment.
     */
    void        push(AvmMemcell _memcell);
    /**
     * @brief Pops an avm memcell from the top of this
     * non empty AvmStackSegment.
     * 
     * @return a read/write access to the popped avm memcell
     * from the top of this non empty AvmStackSegment
     */
    AvmMemcell  pop();
    /**
     * @brief Returns an O(1) access to an inserted 
     * avm memcell.
     * 
     * @param index the index of the memcell, must be
     * in a valid range
     * 
     * @return an O(1) access to an inserted 
     * avm memcell
     */
    AvmMemcell  &operator[](int index);
    /**
     * @brief Returns a read/write access to the avm memcell
     * of a global vmarg, at this non empty AvmStackSegment.
     * 
     * @return a read/write access to the avm memcell
     * of a global vmarg, at this non empty AvmStackSegment
     */
    AvmMemcell  environment(const target_code::GlobalVmarg vmarg) const;
    /**
     * @brief Returns a read/write access to the avm memcell
     * of a local vmarg, at this non empty AvmStackSegment.
     * 
     * @return a read/write access to the avm memcell
     * of a local vmarg, at this non empty AvmStackSegment 
     */
    AvmMemcell  environment(const target_code::LocalVmarg vmarg) const;
    /**
     * @brief Returns a read/write access to the avm memcell
     * of a formal vmarg, at this non empty AvmStackSegment.
     * 
     * @return a read/write access to the avm memcell
     * of a formal vmarg, at this non empty AvmStackSegment 
     */
    AvmMemcell  environment(const target_code::FormalVmarg vmarg) const;
private:
    AvmMemcell  memcells[AVM_STACKSIZE];
    int         _size;
    int         _top;
    bool        full() const;
};
}

#endif