// #include "../include/BlockList.h"

// void
// BlockList::ResetVisibility(void) {
//     BlockList::GetCurrentBlock()->SetVisibilityAll(true);
// }

// void
// BlockList::SetCurrentBlockVisibility(bool _visible) {
//     BlockList::GetCurrentBlock()->SetVisibilityAll(_visible);
// }

// auto
// BlockList::LookUp(const std::string& _id) -> std::pair<const std::string, Variable>* {
//     auto iter = begin();
//     for (; iter != end(); iter++) {
//         auto lkp = (*iter).ids.find(_id);
//         if (lkp != (*iter).ids.end()) {
//             return &*lkp;
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