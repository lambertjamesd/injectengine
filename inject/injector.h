#pragma once

#include <unordered_map>
#include <vector>
#include <exception>

#include "injectorkey.h"
#include "provide.h"
#include "valueprovide.h"

namespace inject {
    class NoProviderFoundExpection : public std::exception {
    public:
        NoProviderFoundExpection(const std::type_info& typeInfo);
        virtual const char* what() const throw();
    private:
        std::string message;
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
                throw NoProviderFoundExpection(typeid(Klass));
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

        void SingleProvide(const Provide* target);
        void Populate(Provide* const* begin, Provide* const* end);
        void Populate(const ProvideEntries& entries);

        struct Entry {
            const Provide& provider;
            void* value;
            bool created;

            Entry(const Provide& provider, void* value, bool created);
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
}