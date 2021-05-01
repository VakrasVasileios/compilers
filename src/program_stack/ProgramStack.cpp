#include "../../include/program_stack/ProgramStack.h"

std::list<Block*>
ProgramStack:: getBlockList() const {
    return blockList;
}

Block*
ProgramStack:: top() {
    return *--blockList.end();
}

void 
ProgramStack:: push(Block* block) {
    blockList.push_back(block);
}

void
ProgramStack:: pop() {
    blockList.pop_back();
}

void
ProgramStack:: activateLowerScopes() {
    auto iter = blockList.begin();
    for (; iter != blockList.end(); ++iter) {
        (*iter)->activate();
    }
}

void
ProgramStack:: deactivateLowerScopes() {
    auto iter = blockList.begin();
    for (; iter != blockList.end(); ++iter) {
        (*iter)->deactivate();
    }
}

SymbolTableEntry*
ProgramStack:: lookup(std::string name) {
    auto iter = blockList.end();
    SymbolTableEntry* found;
    do {
        --iter;
        auto block = (*iter);
        found = block->lookup(name);
        if (found != nullptr)
            return found;
    } while (iter != blockList.begin());

    return nullptr;
}

SymbolTableEntry*
ProgramStack:: lookupGlobal(std::string name) {
    auto iter = blockList.begin();
    auto block = (*iter);

    return block->lookup(name);
}

