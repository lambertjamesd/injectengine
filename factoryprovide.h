#pragma once

#include "provide.h"
#include "injectorkey.h"
#include "injector.h"
#include "indices.h"
#include <tuple>
#include <functional>

template <typename Klass, typename ...Arguments>
class FactoryProvide : public Provide {
public:
    FactoryProvide(std::function<Klass*(Arguments...)> factory, InjectorKey<Arguments>... provideKeys) : 
        Provide(InjectorKey<Klass>::keyForClass()), factory(factory), provideKeys(provideKeys...) {}

    FactoryProvide(const InjectorKey<Klass>& key, std::function<Klass*(Arguments...)> factory, InjectorKey<Arguments>... provideKeys) : 
        Provide(key), factory(factory), provideKeys(provideKeys...) {}

    virtual void* create(Injector& injector) const {
        return createIndexed(injector, build_indices<sizeof...(Arguments)>());
    }

    virtual void destroy(void* value) const {
        delete reinterpret_cast<Klass*>(value);
    }
private:
    template <std::size_t... Is>
    void* createIndexed(Injector& injector, indices<Is...>) const {
        return factory((injector.get(std::get<Is>(provideKeys)))...);
    }

    std::function<Klass*(Arguments...)> factory;
    std::tuple<InjectorKey<Arguments>...> provideKeys;
};