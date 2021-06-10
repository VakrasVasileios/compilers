#include "avm_stack_segment.h"

namespace virtual_machine
{
    namespace stack_segment
    {
    int
    AvmStackSegment::size() const {
        return AVM_STACKSIZE - _top;
    }

    bool
    AvmStackSegment::empty() const {
        return size() == 0;
    }

    memcell::AvmMemcell
    AvmStackSegment::top() const {
        PRECONDITION(!empty());
        return memcells[_top - 1];
    }

    void
    AvmStackSegment::push(memcell::AvmMemcell _memcell) {
        PRECONDITION(!full());
        auto prev__size = size();
        memcells[--_top] = _memcell;
        POSTCONDITION(size() == prev__size + 1);
        POSTCONDITION(!empty());
    }

    memcell::AvmMemcell
    AvmStackSegment::pop() {
        PRECONDITION(!empty());
        auto prev__size = size();
        auto top = memcells[_top++];
        POSTCONDITION(size() == prev__size - 1);
        POSTCONDITION(!full());
        return top;
    }

    memcell::AvmMemcell&
    AvmStackSegment::operator[](int index) {
        PRECONDITION(util::in_range<int>(index, cpu::top+1, AVM_STACKSIZE-1));
        PRECONDITION(!empty());
        return memcells[index];
    }

    memcell::AvmMemcell
    AvmStackSegment::environment(const target_code::GlobalVmarg vmarg) const {
        PRECONDITION(!empty());
        return memcells[cpu::top - vmarg.get_value()];
    }

    memcell::AvmMemcell
    AvmStackSegment::environment(const target_code::LocalVmarg vmarg) const {
        PRECONDITION(!empty());
        return memcells[cpu::topsp - vmarg.get_value()];
    }

    memcell::AvmMemcell
    AvmStackSegment::environment(const target_code::FormalVmarg vmarg) const {
        PRECONDITION(!empty());
        return memcells[cpu::topsp + AVM_STACKENV_SIZE + 1 + vmarg.get_value()];
    }

    bool        
    AvmStackSegment::full() const {
        return size() == AVM_STACKSIZE - 1;
    }
    }
}