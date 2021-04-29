#include "../../include/symbol_table/SymbolTable.h"

std::vector<std::list<Block> >
SymbolTable:: getBlocksTable() const {
    return blocksTable;
}

std::ostream&
operator<<(std::ostream& os, const SymbolTable symbolTable) {
    //     for (int i = 0; i < (*this).size(); i++){
//         std::cout << std::endl;
//         std::cout << "-----------    Scope #" << i << "    -----------" << std::endl;
//         (*this)[i].logBlockList();
//     }
}

// void
// SymbolTable:: insert(unsigned int scope, SymbolTableEntry entry){
//     this->blocksTable[scope].back().addSymbolTableEntry(entry);
// }

// SymbolTableEntry*
// SymbolTable:: lookup(std::string name) {

// }

// SymbolTableEntry
// SymbolTable:: LookUp(SymbolTableEntry entry) {
//     // unsigned int entryScope = entry.getId().getScope();
//     // for (; entryScope >= 0; entryScope--)
//     // {
//     //     std::stack<SymbolTableEntry> temp;
//     //     while (!this->blocksMap[entryScope].empty())
//     //     {
//     //         if (entry != this->blocksMap[entryScope].top().getSymbolTableEntries())
//     //         {
//     //             /* code */
//     //         }
            
//     //     }
        
//     // }    
// }

// void
// SymbolTable:: Hide(int scope) {

// }

// void
// SymbolTable::logSymbolTable(void) {
//     for (int i = 0; i < (*this).size(); i++){
//         std::cout << std::endl;
//         std::cout << "-----------    Scope #" << i << "    -----------" << std::endl;
//         (*this)[i].logBlockList();
//     }
// }