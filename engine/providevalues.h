#pragma once

#include "../inject/injectorkey.h"
#include "../inject/valueprovide.h"
#include <vector>
#include <memory>

namespace engine {
    class ProvideValues {
    public:
        ProvideValues();
        ProvideValues(ProvideValues& other);

        template <typename T>
        void provide(const T& value) {
            provideList.push_back(std::unique_ptr<inject::Provide>(new inject::ValueProvideCopy<T>(value)));
        }

        template <typename T>
        void provide(const inject::InjectorKey<T>& key, const T& value) {
            provideList.push_back(std::unique_ptr<inject::Provide>(new inject::ValueProvideCopy<T>(key, value)));
        }

        inject::ProvideEntries provideEntries();
    private:
        std::vector<std::unique_ptr<inject::Provide>> provideList;
    };
}