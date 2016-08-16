#pragma once

#include "provide.h"

namespace inject {
    template <typename Klass>
    class ValueProvide : public Provide {
    public:
        ValueProvide(Klass& value) : Provide(InjectorKey<Klass>::keyForClass()), value(value) {}
        ValueProvide(const InjectorKey<Klass>& key, Klass& value) : Provide(key), value(value) {}

        virtual void* create(Injector& injector) const {
            return &value;
        }

        virtual void destroy(void* value) const {}
    private:
        Klass& value;
    };

    template <typename Klass>
    class ValueProvideCopy : public Provide {
    public:
        ValueProvideCopy(const Klass& value) : Provide(InjectorKey<Klass>::keyForClass()), value(value) {}
        ValueProvideCopy(const InjectorKey<Klass>& key, Klass& value) : Provide(key), value(value) {}

        virtual void* create(Injector& injector) const {
            return const_cast<Klass*>(&value);
        }

        virtual void destroy(void* value) const {}
    private:
        Klass value;
    };
}