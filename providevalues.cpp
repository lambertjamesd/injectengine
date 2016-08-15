#include "providevalues.h"

ProvideEntries ProvideValues::provideEntries() {
    return ProvideEntries(reinterpret_cast<Provide**>(provideList.data()), provideList.size());
}