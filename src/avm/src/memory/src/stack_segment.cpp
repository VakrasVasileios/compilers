#include "../include/stack_segment.h"

namespace avm
{
    namespace memory
    {        
    int
    StackSegment::size() const {
        INVARIANT(util::range::in_range<int>(AVM_STACKSIZE - registers::top,
            0, AVM_STACKSIZE));
        return AVM_STACKSIZE - registers::top;
    }

    bool
    StackSegment::empty() const {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        return size() == 0;
    }

    bool        
    StackSegment::full() const {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
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
        return memcells[index];
    }

    memcell::AvmMemcell*
    StackSegment::environment(const target_code::GlobalVmarg vmarg) {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(!illegal_index(corresponding_index(vmarg)));
        return memcells[corresponding_index(vmarg)];
    }

    memcell::AvmMemcell*
    StackSegment::environment(const target_code::LocalVmarg vmarg) {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(!illegal_index(corresponding_index(vmarg)));
        return memcells[corresponding_index(vmarg)];
    }

    memcell::AvmMemcell*
    StackSegment::environment(const target_code::FormalVmarg vmarg) {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(!illegal_index(corresponding_index(vmarg)));
        return memcells[corresponding_index(vmarg)];
    }
    bool
    StackSegment::illegal_index(const unsigned int index) const {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(util::range::in_range<int>(index, 0, AVM_STACKSIZE - 1));
        return index < registers::top - 1;
    }

    unsigned            
    StackSegment::corresponding_index(const target_code::GlobalVmarg vmarg)
    const {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        return AVM_STACKSIZE - 1 - vmarg.get_value();
    }

    unsigned            
    StackSegment::corresponding_index(const target_code::LocalVmarg vmarg)
    const {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        return registers::topsp - vmarg.get_value();;
    }

    unsigned            
    StackSegment::corresponding_index(const target_code::FormalVmarg vmarg)
    const {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        return registers::topsp + AVM_STACKENV_SIZE + 1 + vmarg.get_value();
    }

    void                
    StackSegment::push_envvalue(unsigned envvalue) {
        push(new memcell::NumMemcell(envvalue));
    }

    namespace
    {
        memcell::NumMemcell* num_memcell_cast(
            memcell::AvmMemcell* memcell) {
            auto result = dynamic_cast<memcell::NumMemcell*>(memcell);
            PRECONDITION (result != nullptr);
            return result;
        }
    }

    unsigned
    StackSegment::get_envvalue(unsigned index) const {
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        PRECONDITION(index >= 0);
        auto memcell =memcells[index];
        auto result = num_memcell_cast(memcell)->num_val();
        POSTCONDITION(result >= 0);
        INVARIANT(util::range::in_range<int>(size(), 0, AVM_STACKSIZE));
        return result;
    }
    }
}