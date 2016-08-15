#pragma once

#include "injectorkey.h"

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
    ProvideEntries(Provide** begin, Provide** end);
    ProvideEntries(Provide** begin, std::size_t count);
    ProvideEntries(Provide* single);

    ProvideEntries extend(Provide** newBegin, Provide** newEnd);
    ProvideEntries extend(Provide** newBegin, std::size_t count);
    ProvideEntries single(Provide* single);

    ~ProvideEntries();
private:
    ProvideEntries();
    ProvideEntries(Provide** begin, Provide** end, Provide* single, ProvideEntries* additionalEntries);

    Provide** begin;
    Provide** end;
    Provide* singleEntry;
    ProvideEntries* additionalEntries;
    friend class Injector;
};