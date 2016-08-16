#pragma once

#include "provide.h"
#include "injectorkey.h"
#include "injector.h"
#include "indices.h"
#include <tuple>

namespace inject {
    template <typename Klass, typename ...Arguments>
    class ClassProvide : public Provide {
    public:
        ClassProvide(InjectorKey<Arguments>... provideKeys) : Provide(InjectorKey<Klass>::keyForClass()), provideKeys(provideKeys...) {}

        virtual void* create(Injector& injector) const {
            return createIndexed(injector, build_indices<sizeof...(Arguments)>());
        }

        virtual void destroy(void* value) const {
            delete reinterpret_cast<Klass*>(value);
        }
    private:
        template <std::size_t... Is>
        void* createIndexed(Injector& injector, indices<Is...>) const {
            return new Klass((injector.get(std::get<Is>(provideKeys)))...);
        }

        std::tuple<InjectorKey<Arguments>...> provideKeys;
    };


    template <typename Klass, typename Substitute, typename ...Arguments>
    class UseClassProvide : public Provide {
    public:
        UseClassProvide(InjectorKey<Arguments>... provideKeys) : Provide(InjectorKey<Klass>::keyForClass()), provideKeys(provideKeys...) {}

        virtual void* create(Injector& injector) const {
            return createIndexed(injector, build_indices<sizeof...(Arguments)>());
        }

        virtual void destroy(void* value) const {
            delete reinterpret_cast<Substitute*>(value);
        }
    private:
        template <std::size_t... Is>
        void* createIndexed(Injector& injector, indices<Is...>) const {
            return new Substitute((injector.get(std::get<Is>(provideKeys)))...);
        }

        std::tuple<InjectorKey<Arguments>...> provideKeys;
    };
}
