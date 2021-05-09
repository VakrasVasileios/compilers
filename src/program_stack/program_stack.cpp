#include "../../include/program_stack/program_stack.h"

std::list<Block*>
ProgramStack:: get_block_list() const {
    return block_list;
}


Block*
ProgramStack:: Top() {
    return *--block_list.end();
}

void 
ProgramStack:: Push(Block* block) {
    block_list.push_back(block);
}

void
ProgramStack:: Pop() {
    block_list.pop_back();
}

Symbol*
ProgramStack:: Lookup(std::string name) {
    auto iter = block_list.end();
    Symbol* found;
    do {
        --iter;
        auto block = (*iter);
        found = block->Lookup(name);
        if (found != nullptr)
            return found;
    } while (iter != block_list.begin());

    return nullptr;
}

Symbol*
ProgramStack:: LookupGlobal(std::string name) {
    auto iter = block_list.begin();
    auto block = (*iter);

    return block->Lookup(name);
}

Symbol*
ProgramStack:: LookupFunc(std::string name) {
    auto iter = block_list.end();
    Symbol* found;
    do {
        --iter;
        auto block = (*iter);
        found = block->LookupFunc(name);
        if (found != nullptr) {
            return found;
        }
    } while (iter != block_list.begin());

    return nullptr;
}

Function*
ProgramStack:: LookupPreviousFunc() {
    return Top()->LookupPreviousFunc();
}

HiddenVariable*
ProgramStack:: LookupHiddenVariable(std::string name) {
    return Top()->LookupHiddenVariable(name);
}

void
ProgramStack:: ActivateLowerScopes() {
    auto iter = block_list.begin();
    for (; iter != block_list.end(); ++iter) {
        (*iter)->Activate();
    }
}

void
ProgramStack:: DeactivateLowerScopes() {
    auto iter = block_list.begin();
    for (; iter != block_list.end(); ++iter) {
        (*iter)->Deactivate();
    }
}
