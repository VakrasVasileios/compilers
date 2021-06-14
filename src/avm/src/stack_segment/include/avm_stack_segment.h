#ifndef AVM_STACK_SEGMENT_H
#define AVM_STACK_SEGMENT_H

#include <vector>
#include "../../../../../util/contract/include/contract.h"
#include "../../../../../util/range/include/range.h"
#include "../../../../target_code/include/vm_arg.h"
#include "../../memcell/include/avm_memcell.h"

#define AVM_STACKSIZE       4096
#define AVM_STACKENV_SIZE   4

namespace avm
{
    namespace stack_segment 
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
      AvmStackSegment() : top_(AVM_STACKSIZE), topsp_(top_)
      {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
      }
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
      int                 size() const;
      /**
       * @brief Checks wether this AvmStackSegment is empty.
       * 
       * @return  wether this AvmStackSegment is empty
       */
      bool                empty() const;
      /**                                                    
       * @brief Returns a read/write access to the top
       * avm memcell of this non empty AvmStackSegment.
       * 
       * @return a read/write access to the top
       * avm memcell of non empty this AvmStackSegment 
       */
      memcell::AvmMemcell*  top();
      /**
       * @brief Pushes a memcell at the top of this
       * AvmStackSegment.
       * 
       * @param _memcell the memcell to be pushed
       * at the top of this AvmStackSegment.
       */
      void                  push(memcell::AvmMemcell* _memcell);
      /**
       * @brief Pops an avm memcell from the top of this
       * non empty AvmStackSegment.
       * 
       * @return a read/write access to the popped avm memcell
       * from the top of this non empty AvmStackSegment
       */
      memcell::AvmMemcell*  pop();
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
      memcell::AvmMemcell* operator[](int index);
      /**
       * @brief Returns a read/write access to the avm memcell
       * of a global vmarg, at this AvmStackSegment.
       * In case that the vmarg is not pushed, pushes it
       * to this AvmStackSegment.
       * 
       * @param vmarg the global vmarg to be accessed
       * and/or pushed
       * 
       * @return a read/write access to the avm memcell
       * of a global vmarg, at this AvmStackSegment
       */
      memcell::AvmMemcell*
        environment(const target_code::GlobalVmarg vmarg);
      /**
       * @brief Returns a read/write access to the avm memcell
       * of a local vmarg, at this AvmStackSegment.
       * In case that the vmarg is not pushed, pushes it
       * to this AvmStackSegment.
       * 
       * @param vmarg the local vmarg to be accessed
       * and/or pushed
       * 
       * @return a read/write access to the avm memcell
       * of a local vmarg, at this AvmStackSegment 
       */
      memcell::AvmMemcell*
        environment(const target_code::LocalVmarg vmarg);
      /**
       * @brief Returns a read/write access to the avm memcell
       * of a formal vmarg, at this AvmStackSegment.
       * In case that the vmarg is not pushed, pushes it
       * to this AvmStackSegment.
       * 
       * @param vmarg the formal vmarg to be accessed
       * and/or pushed
       * 
       * @return a read/write access to the avm memcell
       * of a formal vmarg, at this AvmStackSegment 
       */
      memcell::AvmMemcell*
        environment(const target_code::FormalVmarg vmarg);
    private:
      memcell::AvmMemcell*  memcells[AVM_STACKSIZE];
      unsigned              top_;
      unsigned              topsp_;
      bool                  full() const;
      bool                  index_out_of_bounds(const unsigned int index) const;
    };
    }
}

#endif