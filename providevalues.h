#pragma once

#include "injectorkey.h"
#include "valueprovide.h"
#include <vector>
#include <memory>

class ProvideValues {
public:
    template <typename T>
    void provide(const T& value) {
        provideList.push_back(new ValueProvideCopy<T>(value));
    }

    template <typename T>
    void provide(const InjectorKey<T>& key, const T& value) {
        provideList.push_back(new ValueProvideCopy<T>(key, value));
    }

    ProvideEntries provideEntries();
private:
    std::vector<std::unique_ptr<Provide>> provideList;
};