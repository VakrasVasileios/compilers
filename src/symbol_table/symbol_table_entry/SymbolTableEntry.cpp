#include "SymbolTableEntry.h"

SymbolType
SymbolTableEntry:: getType() const {
    return type;
}

Id
SymbolTableEntry:: getId() const {
    return id;
}

bool
SymbolTableEntry:: isActive() const {
    return active;
}

void
SymbolTableEntry:: setActive(bool active) {
    this->active = active;
}

std::ostream&
operator<<(std::ostream& os, const SymbolTableEntry symbolTableEntry) {
    return os << "[" << symbolTableEntry.getType() << "]" << " " << symbolTableEntry.getId();
}