#pragma once

#include <unordered_map>
#include <vector>

#include "injectorkey.h"
#include "provide.h"
#include "valueprovide.h"

class NoProviderFoundExpection {
public:
    NoProviderFoundExpection();
};

class Injector {
public:
    template <typename Klass>
    Klass& get() {
        return get(InjectorKey<Klass>::keyForClass());
    }

    template <typename Klass>
    Klass* getOptional() {
        return get(InjectorKey<Klass>::keyForClass().optional());
    }

    template <typename Klass>
    Klass& get(const InjectorKey<Klass>& key) {
        Klass* result = get(key.optional());

        if (result) {
            return *result;
        } else {
            throw new NoProviderFoundExpection();
        }
    }
    template <typename Klass>
    Klass* get(const InjectorKey<Klass*>& key) {
        return reinterpret_cast<Klass*>(getRaw(key));
    }

    void* getRaw(const InjectorKeyUntyped& key);

    Injector* newChildInjector(ProvideEntries entries);

    Injector(ProvideEntries entries);
    Injector(ProvideEntries entries, Injector* parent);
    ~Injector();

    static InjectorKey<Injector> KEY;
private:
    Injector();

    void SingleProvide(Provide* target);
    void Populate(Provide** begin, Provide** end);
    void Populate(ProvideEntries& entries);

    struct Entry {
        Provide& provider;
        void* value;
        bool created;

        Entry(Provide& provider, void* value, bool created);
    };

    Injector* parent;
    Injector* firstChild;
    Injector* nextSibling;
    Injector* prevSibling;

    std::unordered_map<InjectorKeyUntyped, Entry> provideMapping;
    std::vector<InjectorKeyUntyped> constructOrder;
    ValueProvide<Injector> selfProvide;

    void RemoveChild(Injector* injector);
    void AddChild(Injector* injector);
};