// #include "../include/BlockList.h"

// void
// BlockList::resetVisibility(void) {
//     BlockList::getCurrentBlock()->setVisibilityAll(true);
// }

// void
// BlockList::setCurrentBlockVisibility(bool visible) {
//     BlockList::getCurrentBlock()->setVisibilityAll(visible);
// }

// auto
// BlockList::lookUp(const std::string name) {
//     auto iter = begin();
//     for (; iter != end(); iter++) {
//         auto lkp = (*iter).entries.find(name);
//         if (lkp != (*iter).entries.end()) {
//             return &(*lkp);
//         }
//     }
//     return nullptr;
// }

// void
// BlockList::logBlockList(void) {
//     for (auto iter : (*this)) {
//         iter.logBlock();
//     }
// }