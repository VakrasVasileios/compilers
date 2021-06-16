#include "../include/stack_segment.h"

namespace avm
{
    namespace memory
    {
    int
    StackSegment::size() const {
        return AVM_STACKSIZE - registers::top;
    }

    bool
    StackSegment::empty() const {
        return size() == 0;
    }

    bool        
    StackSegment::full() const {
        return size() == AVM_STACKSIZE;
    }

    memcell::AvmMemcell*
    StackSegment::top() {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(!empty());
        return memcells[registers::top];
    }

    void
    StackSegment::push(memcell::AvmMemcell* _memcell) {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(!full());
        memcells[--registers::top] = _memcell;
        POSTCONDITION(!empty());
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
    }

    memcell::AvmMemcell*
    StackSegment::pop() {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(!empty());
        auto top = StackSegment::top();
        delete memcells[registers::top++];
        POSTCONDITION(!full());
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        return top;
    }

    memcell::AvmMemcell*
    StackSegment::operator[](int index) {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(util::range::in_range<int>(index, 0, size() - 1));
        return memcells[AVM_STACKSIZE - 1 - index];
    }

    namespace
    {
    bool index_out_of_bounds(const unsigned int index) {
        assert(util::range::in_range<int>(index, 0, AVM_STACKSIZE - 1));
        return index < registers::top;
    }
    }

    memcell::AvmMemcell*
    StackSegment::environment(const target_code::GlobalVmarg vmarg) {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        auto global_vmarg_index = AVM_STACKSIZE - 1 - vmarg.get_value();
        if (index_out_of_bounds(global_vmarg_index))
            registers::top = global_vmarg_index; // SEG FAULT?
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));   
        return memcells[global_vmarg_index];
    }

    memcell::AvmMemcell*
    StackSegment::environment(const target_code::LocalVmarg vmarg) {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        auto local_vmarg_index = registers::topsp - vmarg.get_value();
        if (index_out_of_bounds(local_vmarg_index))
            registers::top = local_vmarg_index;  // SEG FAULT?
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        return memcells[local_vmarg_index];
    }

    memcell::AvmMemcell*
    StackSegment::environment(const target_code::FormalVmarg vmarg) {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        auto formal_vmarg_index = registers::topsp + AVM_STACKENV_SIZE + 1 + 
            vmarg.get_value();
        if (index_out_of_bounds(formal_vmarg_index))
            registers::top = formal_vmarg_index;  // SEG FAULT?
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));    
        return memcells[formal_vmarg_index];
    }
    }
}