#ifndef STACK_SEGMENT_H
#define STACK_SEGMENT_H

#include <vector>
#include "../../../../../util/contract/include/contract.h"
#include "../../../../../util/range/include/range.h"
#include "../../../../target_code/include/vm_arg.h"
#include "../../memcell/include/avm_memcell.h"
#include "../../registers/include/registers.h"

namespace avm
{
    namespace memory 
    {
    /**
     * @brief The avm stack segment containing the global
     * variables and the call stack section.
     * 
     */
    class StackSegment final {
    public:
      /**
       * @brief Constructs a new StackSegment object.
       * 
       */
      StackSegment()
      {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
      }
      /**
       * @brief Destroys this StackSegment object.
       * 
       */
      ~StackSegment() = default;
      /**
       * @brief Returns a read access to this StackSegment
       * size.
       * 
       * @return a read access to this StackSegment
       * size
       */
      int                 size() const;
      /**
       * @brief Checks wether this StackSegment is empty.
       * 
       * @return  wether this StackSegment is empty
       */
      bool                empty() const;
      /**
       * @brief Checks wether this StackSegment is full.
       * 
       * @return wether this StackSegment is full 
       */
      bool                full() const;                       
      /**                                                    
       * @brief Returns a read/write access to the top
       * avm memcell of this non empty StackSegment.
       * 
       * @return a read/write access to the top
       * avm memcell of non empty this StackSegment 
       */
      memcell::AvmMemcell*  top();
      /**
       * @brief Pushes a memcell at the top of this
       * StackSegment.
       * 
       * @param _memcell the memcell to be pushed
       * at the top of this StackSegment.
       */
      void                  push(memcell::AvmMemcell* _memcell);
      /**
       * @brief Pops an avm memcell from the top of this
       * non empty StackSegment.
       * 
       * @return a read/write access to the popped avm memcell
       * from the top of this non empty StackSegment
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
       * of a global vmarg, at this StackSegment.
       * 
       * @param vmarg the global vmarg to be accessed,
       * can't correspond to an index that is 2 or more 
       * positions out of bounds
       * 
       * @return a read/write access to the avm memcell
       * of a global vmarg, at this StackSegment
       */
      memcell::AvmMemcell** environment(const target_code::GlobalVmarg vmarg);
      /**
       * @brief Returns a read/write access to the avm memcell
       * of a local vmarg, at this StackSegment.
       * 
       * @param vmarg the local vmarg to be accessed,
       * can't correspond to an index that is 2 or more 
       * positions out of bounds
       * 
       * @return a read/write access to the avm memcell
       * of a local vmarg, at this StackSegment 
       */
      memcell::AvmMemcell** environment(const target_code::LocalVmarg vmarg);
      /**
       * @brief Returns a read/write access to the avm memcell
       * of a formal vmarg, at this StackSegment.
       * 
       * @param vmarg the formal vmarg to be accessed,
       * can't correspond to an index that is 2 or more 
       * positions out of bounds
       * 
       * @return a read/write access to the avm memcell
       * of a formal vmarg, at this StackSegment 
       */
      memcell::AvmMemcell** environment(const target_code::FormalVmarg vmarg);
      /**
       * @brief Checks wether a given index is 2 or more 
       * positions out of bounds of this StackSegment.
       * 
       * @param index the index to check wether it's 
       * 2 or more positions out of bounds, must be in range
       * of the defined stack size
       * 
       * @return wether a given index is 2 or more 
       * positions out of bounds of this StackSegment
       */
      bool                illegal_index(const unsigned int index) const;
      /**
       * @brief Returns a read access to the corresponding index
       * of a local target code vmarg.
       * 
       * @param vmarg the local target code vmarg
       * 
       * @return a read access to the corresponding index
       * of a local target code vmarg
       */
      unsigned            corresponding_index(const
                            target_code::LocalVmarg vmarg) const;
      /**
       * @brief Returns a read access to the corresponding index
       * of a global target code vmarg.
       * 
       * @param vmarg the global target code vmarg
       * 
       * @return a read access to the corresponding index
       * of a global target code vmarg
       */                      
      unsigned            corresponding_index(const
                            target_code::GlobalVmarg vmarg) const; 
      /**
       * @brief Returns a read access to the corresponding index
       * of a formal target code vmarg.
       * 
       * @param vmarg the formal target code vmarg
       * 
       * @return a read access to the corresponding index
       * of a formal target code vmarg
       */                       
      unsigned            corresponding_index(const
                            target_code::FormalVmarg vmarg) const; 
      /**
       * @brief Pushes an environment value to this StackSegment.
       * 
       * @param envvalue the environment value
       */
      void                push_envvalue(unsigned envvalue);                      
      /**
       * @brief Returns a read access to a previosuly stored
       * environment value.
       * 
       * @param index the index of the previously stored environment
       * value, greater or equal to zero
       * 
       * @return  a read access to a previosuly stored
       * environment value, greater or equal to zero
       */
      unsigned            get_envvalue(unsigned index) const;    
      /**
       * @brief Clears this StackSegment.
       * 
       */
      void                clear();
      friend std::ostream& operator<<(std::ostream& os, const StackSegment& st) {
          for (int i = AVM_STACKSIZE-st.size(); i < AVM_STACKSIZE; i++)
            os << *(st.memcells[i]) << std::endl;
        return os;
      }
    private:
      memcell::AvmMemcell*  memcells[AVM_STACKSIZE];
    };
    }
}

#endif