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

    memcell::AvmMemcell
    AvmStackSegment::environment(const target_code::GlobalVmarg vmarg) {
        INVARIANT(util::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(!empty());
        return memcells[AVM_STACKSIZE - 1 - vmarg.get_value()];
    }

    memcell::AvmMemcell
    AvmStackSegment::environment(const target_code::LocalVmarg vmarg) {
        INVARIANT(util::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(!empty());
        return memcells[cpu::topsp - vmarg.get_value()];
    }

    memcell::AvmMemcell
    AvmStackSegment::environment(const target_code::FormalVmarg vmarg) {
        INVARIANT(util::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(!empty());
        return memcells[cpu::topsp + AVM_STACKENV_SIZE + 1 + vmarg.get_value()];
    }

    bool        
    AvmStackSegment::full() const {
        return size() == AVM_STACKSIZE;
    }
    }
}