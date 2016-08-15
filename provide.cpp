#include "provide.h"

Provide::Provide(const InjectorKeyUntyped& key) : key(key) {}

const InjectorKeyUntyped& Provide::getKey() const {
    return key;
}

ProvideEntries::ProvideEntries(Provide** begin, Provide** end) :
    begin(begin), end(end), singleEntry(0), additionalEntries(0) {

}

ProvideEntries::ProvideEntries(Provide** begin, std::size_t count) : 
    begin(begin), end(begin + count), singleEntry(0), additionalEntries(0) {

}

ProvideEntries::ProvideEntries(Provide* single) :
    begin(0), end(0), singleEntry(single), additionalEntries(0) {

}

ProvideEntries::ProvideEntries(Provide** begin, Provide** end, Provide* single, ProvideEntries* additionalEntries) :
    begin(begin), end(end), singleEntry(single), additionalEntries(additionalEntries) {

}

ProvideEntries ProvideEntries::extend(Provide** newBegin, Provide** newEnd) {
    return ProvideEntries(newBegin, newEnd, 0, this);
}

ProvideEntries ProvideEntries::extend(Provide** newBegin, std::size_t count) {
    return ProvideEntries(newBegin, newBegin + count, 0, this);
}

ProvideEntries ProvideEntries::single(Provide* single) {
    return ProvideEntries(0, 0, single, this);
}

ProvideEntries::~ProvideEntries() {

}