#pragma once

#include "injectorkey.h"

namespace inject {
    class Injector;

    class Provide {
    public:
        virtual void* create(Injector& injector) const = 0;
        virtual void destroy(void* value) const = 0;

        const InjectorKeyUntyped& getKey() const;
    protected:
        Provide(const InjectorKeyUntyped& key);
    private:
        Provide();
        InjectorKeyUntyped key;
    };

    class ProvideEntries {
    public:
        ProvideEntries(Provide* const* begin, Provide* const* end);
        ProvideEntries(Provide* const* begin, std::size_t count);
        ProvideEntries(const Provide* single);

        ProvideEntries extend(Provide* const* newBegin, Provide* const* newEnd);
        ProvideEntries extend(Provide* const* newBegin, std::size_t count);
        ProvideEntries single(const Provide* single);
        ProvideEntries join(const ProvideEntries& other);

        ~ProvideEntries();
    private:
        ProvideEntries();
        ProvideEntries(Provide* const* begin, Provide* const* end, const Provide* single, const ProvideEntries* additionalEntries, const ProvideEntries* branchingEntries);

        Provide* const* begin;
        Provide* const* end;
        const Provide* singleEntry;
        const ProvideEntries* additionalEntries;
        const ProvideEntries* branchingEntries;
        friend class Injector;
    };
}