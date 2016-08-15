#pragma once

#include <typeinfo>
#include <typeindex>

class InjectorKeyUntyped {
public:
    bool operator==(const InjectorKeyUntyped& other) const;
    std::size_t hash_code() const;

    bool shouldSkipSelf() const;
    bool shouldSkipParent() const;
protected:
    InjectorKeyUntyped(const std::type_index& klassType, std::size_t index, int flags);
    
    std::type_index klassType;
    std::size_t index;
    int flags;

    static std::size_t nextGlobalIndex();
    static std::size_t globalIndex;

    enum InjectorKeyFlags {
        SKIP_SELF = 1,
        SKIP_PARENT = 2,
    };
};

namespace std {
    template <>
    struct hash<InjectorKeyUntyped> {
        std::size_t operator()(const InjectorKeyUntyped& key) const {
            return key.hash_code();
        }
    };
}

template<typename Klass>
class InjectorKey : public InjectorKeyUntyped {
public:
    InjectorKey<Klass*> optional() const {
        return InjectorKey<Klass*>(klassType, index, flags);
    }

    InjectorKey<Klass> skipParent() const {
        return InjectorKey<Klass>(klassType, index, flags | InjectorKeyUntyped::SKIP_PARENT);
    }

    InjectorKey<Klass> skipSelf() const {
        return InjectorKey<Klass>(klassType, index, flags | InjectorKeyUntyped::SKIP_SELF);
    }

    static InjectorKey<Klass> keyForClass() {
        return InjectorKey<Klass>(std::type_index(typeid(Klass)), static_cast<std::size_t>(0), 0);
    }

    static InjectorKey<Klass> uniqueKeyForClass() {
        return InjectorKey<Klass>(std::type_index(typeid(Klass)), InjectorKeyUntyped::nextGlobalIndex(), 0);
    }

    InjectorKey(std::size_t index) : InjectorKeyUntyped(std::type_index(typeid(Klass)), index, 0) {}
    InjectorKey() : InjectorKeyUntyped(std::type_index(typeid(Klass)), static_cast<std::size_t>(0), 0) {}
    InjectorKey(const std::type_index& klassType, std::size_t index, int flags) : InjectorKeyUntyped(klassType, index, flags) {}
private:
};