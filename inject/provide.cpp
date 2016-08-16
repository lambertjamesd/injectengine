#include "provide.h"

namespace inject {
    Provide::Provide(const InjectorKeyUntyped& key) : key(key) {}

    const InjectorKeyUntyped& Provide::getKey() const {
        return key;
    }

    ProvideEntries::ProvideEntries(Provide* const* begin, Provide* const* end) :
        begin(begin), end(end), singleEntry(0), additionalEntries(0), branchingEntries(0) {

    }

    ProvideEntries::ProvideEntries(Provide* const* begin, std::size_t count) : 
        begin(begin), end(begin + count), singleEntry(0), additionalEntries(0), branchingEntries(0) {

    }

    ProvideEntries::ProvideEntries(const Provide* single) :
        begin(0), end(0), singleEntry(single), additionalEntries(0), branchingEntries(0) {

    }

    ProvideEntries::ProvideEntries(Provide* const* begin, Provide* const* end, const Provide* single, const ProvideEntries* additionalEntries, const ProvideEntries* branchingEntries) :
        begin(begin), end(end), singleEntry(single), additionalEntries(additionalEntries), branchingEntries(branchingEntries) {

    }

    ProvideEntries ProvideEntries::extend(Provide* const* newBegin, Provide* const* newEnd) {
        return ProvideEntries(newBegin, newEnd, 0, this, 0);
    }

    ProvideEntries ProvideEntries::extend(Provide* const* newBegin, std::size_t count) {
        return ProvideEntries(newBegin, newBegin + count, 0, this, 0);
    }

    ProvideEntries ProvideEntries::single(const Provide* single) {
        return ProvideEntries(0, 0, single, this, 0);
    }

    ProvideEntries ProvideEntries::join(const ProvideEntries& other) {
        return ProvideEntries(0, 0, 0, this, &other);
    }

    ProvideEntries::~ProvideEntries() {

    }
}