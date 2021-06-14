#include "../include/avm_stack_segment.h"

namespace avm
{
    namespace stack_segment
    {
    int
    AvmStackSegment::size() const {
        return AVM_STACKSIZE - cpu::top;
    }

    bool
    AvmStackSegment::empty() const {
        return size() == 0;
    }

    memcell::AvmMemcell*
    AvmStackSegment::top() {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(!empty());
        return memcells[cpu::top];
    }

    void
    AvmStackSegment::push(memcell::AvmMemcell* _memcell) {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(!full());
        memcells[--cpu::top] = _memcell;
        POSTCONDITION(!empty());
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
    }

    memcell::AvmMemcell*
    AvmStackSegment::pop() {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(!empty());
        auto top = memcells[cpu::top++];
        POSTCONDITION(!full());
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        return top;
    }

    memcell::AvmMemcell*&
    AvmStackSegment::operator[](int index) {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(util::range::in_range<int>(index, 0, size() - 1));
        return memcells[size() - 1 - index];
    }

    namespace 
    {
    bool index_out_of_bounds(const unsigned int index) {
        assert(util::range::in_range<int>(index, 0, AVM_STACKSIZE - 1));
        return index < cpu::top;
    }
    }

    memcell::AvmMemcell*
    AvmStackSegment::environment(const target_code::GlobalVmarg vmarg) {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        auto global_vmarg_index = AVM_STACKSIZE - 1 - vmarg.get_value();
        if (index_out_of_bounds(global_vmarg_index))
            cpu::top = global_vmarg_index;
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));    
        return memcells[global_vmarg_index];
    }

    memcell::AvmMemcell*
    AvmStackSegment::environment(const target_code::LocalVmarg vmarg) {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        auto local_vmarg_index = cpu::topsp - vmarg.get_value();
        if (index_out_of_bounds(local_vmarg_index))
            cpu::top = local_vmarg_index;
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        return memcells[local_vmarg_index];
    }

    memcell::AvmMemcell*
    AvmStackSegment::environment(const target_code::FormalVmarg vmarg) {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        auto formal_vmarg_index = cpu::topsp + AVM_STACKENV_SIZE + 1 + 
            vmarg.get_value();
        if (index_out_of_bounds(formal_vmarg_index))
            cpu::top = formal_vmarg_index;
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));    
        return memcells[formal_vmarg_index];
    }

    bool        
    AvmStackSegment::full() const {
        return size() == AVM_STACKSIZE;
    }
    }
}