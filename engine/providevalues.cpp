#include "providevalues.h"

namespace engine {
    ProvideValues::ProvideValues() {

    }

    ProvideValues::ProvideValues(ProvideValues& other) {
        for (std::size_t i = 0; i < other.provideList.size(); ++i) {
            provideList.push_back(std::move(other.provideList[i]));
        }

        other.provideList.empty();
    }

    inject::ProvideEntries ProvideValues::provideEntries() {
        return inject::ProvideEntries(reinterpret_cast<inject::Provide**>(provideList.data()), provideList.size());
    }
}