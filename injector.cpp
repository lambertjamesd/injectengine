#include "injector.h"

NoProviderFoundExpection::NoProviderFoundExpection() {}

InjectorKey<Injector> Injector::KEY;

Injector::Injector() :
    parent(0),
    firstChild(0),
    nextSibling(0),
    prevSibling(0),
    selfProvide(Injector::KEY, *this) {
    SingleProvide(&selfProvide);
}

Injector::Injector(ProvideEntries entries) : Injector() {
    Populate(entries);
}

Injector::Injector(ProvideEntries entries, Injector* parent) : Injector() {
    Populate(entries);

    if (parent) {
        this->parent = parent;
        parent->AddChild(this);
    }
}

Injector::Entry::Entry(Provide& provider, void* value, bool created) : 
    provider(provider), 
    value(value), 
    created(created) {

}

void Injector::SingleProvide(Provide* target) {
    provideMapping.insert(std::make_pair(target->getKey(), Entry(*target, 0, false)));
}

void Injector::Populate(Provide** begin, Provide** end) {
    for (Provide** it = begin; it != end; ++it) {
        SingleProvide(*it);
    }
}

void Injector::Populate(ProvideEntries& entries) {
    if (entries.additionalEntries) {
        Populate(*entries.additionalEntries);
    }
    Populate(entries.begin, entries.end);
    if (entries.singleEntry) {
        SingleProvide(entries.singleEntry);
    }
}

void* Injector::getRaw(const InjectorKeyUntyped& key) {
    auto location = provideMapping.find(key);

    if (location != provideMapping.end() && !key.shouldSkipSelf()) {
        if (!location->second.created) {
            location->second.value = location->second.provider.create(*this);
            location->second.created = true;
            constructOrder.push_back(key);
        }

        return location->second.value;
    } else if (parent && !key.shouldSkipParent()) {
        return parent->getRaw(key);
    } else {
        return NULL;
    }
}

Injector* Injector::newChildInjector(ProvideEntries entries) {
    Injector* result = new Injector(entries);
    AddChild(result);
    return result;
}

Injector::~Injector() {
    while (firstChild) {
        delete firstChild;
    }

    if (parent) {
        parent->RemoveChild(this);
    }

    for (auto it = constructOrder.rbegin(); it != constructOrder.rend(); ++it) {
        auto provide = provideMapping.find(*it);

        if (provide != provideMapping.end()) {
            provide->second.provider.destroy(provide->second.value);
        }
    }
}

void Injector::RemoveChild(Injector* injector) {
    Injector* next = injector->nextSibling;
    Injector* prev = injector->prevSibling;

    if (next) {
        next->prevSibling = prev;
    }

    if (prev) {
        prev->nextSibling = next;
    }

    if (injector == firstChild) {
        firstChild = injector->nextSibling;
    }

    injector->parent = 0;
    injector->firstChild = 0;
    injector->nextSibling = 0;
    injector->prevSibling = 0;
}

void Injector::AddChild(Injector* injector) {
    injector->parent = this;
    injector->nextSibling = firstChild;

    if (firstChild) {
        firstChild->prevSibling = injector;
    }

    firstChild = injector;
}