#include "injectorkey.h"

InjectorKeyUntyped::InjectorKeyUntyped(const std::type_index& klassType, std::size_t index, int flags) : 
    klassType(klassType), 
    index(index),
    flags(flags) {

}

std::size_t InjectorKeyUntyped::globalIndex = ~0;

std::size_t InjectorKeyUntyped::nextGlobalIndex() {
    return globalIndex--;
}

bool InjectorKeyUntyped::operator==(const InjectorKeyUntyped& other) const {
    return klassType == other.klassType && index == other.index;
}

std::size_t InjectorKeyUntyped::hash_code() const {
    return klassType.hash_code() ^ index;
}

bool InjectorKeyUntyped::shouldSkipSelf() const {
    return (flags & InjectorKeyUntyped::SKIP_SELF) != 0; 
}

bool InjectorKeyUntyped::shouldSkipParent() const {
    return (flags & InjectorKeyUntyped::SKIP_PARENT) != 0;
}