#pragma once

#include "injector.h"
#include "gameobjectstore.h"
#include "injectorkey.h"
#include "valueprovide.h"

class GameObject {
public:
    static GameObject* createRoot(ProvideEntries entries);
    GameObject* createChild(ProvideEntries entries);

    static InjectorKey<GameObject> KEY;

    WeakRef<GameObject> weakRef();

    ~GameObject();
private:
    friend class WeakRef<GameObject>;

    GameObject(ProvideEntries entries);
    GameObject(ProvideEntries entries, GameObject& parent);

    ValueProvide<GameObject> selfProvide;
    Injector injector;
    GameObjectStore& objectStore;
    GameObjectId id;
};

template <>
class WeakRef<GameObject> {
public:
    WeakRef(GameObjectStore& objectStore, GameObjectId id, GameObject* pointer) :
        objectStore(objectStore),
        id(id),
        pointer(pointer) {

    }

    bool exists() const {
        return pointer && objectStore.exists(id);
    }

    GameObject* lock() {
        if (exists()) {
            return pointer;
        } else {
            return NULL;
        }
    }

    template <typename Klass>
    WeakRef<Klass> getComponent() const {
        if (exists()) {
            return WeakRef<Klass>(objectStore, id, pointer->injector.getOptional<Klass>());
        } else {
            return WeakRef<Klass>(objectStore, id, 0);
        }
    }

    template <typename Klass>
    WeakRef<Klass> getComponent(const InjectorKey<Klass>& key) const {
        if (exists()) {
            return WeakRef<Klass>(objectStore, id, pointer->injector.get(key.optional()));
        } else {
            return WeakRef<Klass>(objectStore, id, 0);
        }
    }
private:
    GameObjectStore& objectStore;
    GameObjectId id;
    GameObject* pointer;
};