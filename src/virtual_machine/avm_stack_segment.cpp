#include "avm_stack_segment.h"

namespace virtual_machine
{
AvmMemcell
AvmStackSegment::top() const {
    return memcells.back();
}

void
AvmStackSegment::push(AvmMemcell _memcell) {
    auto prev_size = memcells.size();
    memcells.push_back(_memcell);
    auto new_size = memcells.size();
    POSTCONDITION(new_size == prev_size + 1);
}

AvmMemcell
AvmStackSegment::pop() {
    PRECONDITION(!memcells.empty());
    auto prev_size = memcells.size();
    auto top = this->top();
    memcells.pop_back();
    auto new_size = memcells.size();
    POSTCONDITION(new_size == prev_size - 1);
    return top;
}

AvmMemcell&
AvmStackSegment::operator[](int index) {
    PRECONDITION(util::in_range<int>(index, 0, memcells.size() - 1));
    return memcells[index];
}

AvmMemcell
AvmStackSegment::environment(const target_code::GlobalVmarg vmarg) const {
    PRECONDITION(!memcells.empty());
    return memcells[cpu::top - vmarg.get_value()];
}

AvmMemcell
AvmStackSegment::environment(const target_code::LocalVmarg vmarg) const {
    PRECONDITION(!memcells.empty());
    return memcells[cpu::topsp - vmarg.get_value()];
}

AvmMemcell
AvmStackSegment::environment(const target_code::FormalVmarg vmarg) const {
    PRECONDITION(!memcells.empty());
    return memcells[cpu::topsp + AVM_STACKENV_SIZE + 1 + vmarg.get_value()];
}

}