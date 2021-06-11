#include "avm_stack_segment.h"

namespace virtual_machine
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

    memcell::AvmMemcell
    AvmStackSegment::top() {
        INVARIANT(util::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(!empty());
        return memcells[cpu::top];
    }

    void
    AvmStackSegment::push(memcell::AvmMemcell _memcell) {
        INVARIANT(util::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(!full());
        memcells[--cpu::top] = _memcell;
        POSTCONDITION(!empty());
        INVARIANT(util::in_range<int>(size(), 0, AVM_STACKSIZE));
    }

    memcell::AvmMemcell
    AvmStackSegment::pop() {
        INVARIANT(util::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(!empty());
        auto top = memcells[cpu::top++];
        POSTCONDITION(!full());
        INVARIANT(util::in_range<int>(size(), 0, AVM_STACKSIZE));
        return top;
    }

    memcell::AvmMemcell&
    AvmStackSegment::operator[](int index) {
        INVARIANT(util::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(util::in_range<int>(index, 0, size() - 1));
        return memcells[size() - 1 - index];
    }

    namespace 
    {
    bool index_in_bounds(const unsigned int index) {
        assert(util::in_range<int>(index, 0, AVM_STACKSIZE - 1));
        return index < cpu::top;
    }
    }

    memcell::AvmMemcell
    AvmStackSegment::environment(const target_code::GlobalVmarg vmarg) {
        INVARIANT(util::in_range<int>(size(), 0, AVM_STACKSIZE));
        auto global_vmarg_index = AVM_STACKSIZE - 1 - vmarg.get_value();
        if (index_in_bounds(global_vmarg_index))
            push(memcell::AvmMemcell());
        INVARIANT(util::in_range<int>(size(), 0, AVM_STACKSIZE));    
        return memcells[global_vmarg_index];
    }

    memcell::AvmMemcell
    AvmStackSegment::environment(const target_code::LocalVmarg vmarg) {
        INVARIANT(util::in_range<int>(size(), 0, AVM_STACKSIZE));
        auto local_vmarg_index = cpu::topsp - vmarg.get_value();
        if (index_in_bounds(local_vmarg_index))
            push(memcell::AvmMemcell());
        INVARIANT(util::in_range<int>(size(), 0, AVM_STACKSIZE));
        return memcells[local_vmarg_index];
    }

    memcell::AvmMemcell
    AvmStackSegment::environment(const target_code::FormalVmarg vmarg) {
        INVARIANT(util::in_range<int>(size(), 0, AVM_STACKSIZE));
        auto formal_vmarg_index = cpu::topsp + AVM_STACKENV_SIZE + 1 + vmarg.get_value();
        if (index_in_bounds(formal_vmarg_index))
            push(memcell::AvmMemcell());
        INVARIANT(util::in_range<int>(size(), 0, AVM_STACKSIZE));    
        return memcells[formal_vmarg_index];
    }

    bool        
    AvmStackSegment::full() const {
        return size() == AVM_STACKSIZE;
    }
    }
}