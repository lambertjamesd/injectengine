#pragma once

#include "../inject/injector.h"
#include "gameobjectstore.h"
#include "../inject/injectorkey.h"
#include "../inject/valueprovide.h"
#include "providevalues.h"

namespace engine {
    class GameObject {
    public:
        static GameObject* createRoot(inject::ProvideEntries entries);
        static GameObject* createRoot(inject::ProvideEntries entries, ProvideValues& provideValues);
        GameObject* createChild(inject::ProvideEntries entries);
        GameObject* createChild(inject::ProvideEntries entries, ProvideValues& provideValues);

        static inject::InjectorKey<GameObject> KEY;

        WeakRef<GameObject> weakRef();

        template <typename T>
        WeakRef<T> getComponent() {
            return WeakRef<T>(objectStore, id, injector.getOptional<T>());
        }

        ~GameObject();
    private:
        friend class WeakRef<GameObject>;

        GameObject(inject::ProvideEntries entries, ProvideValues& provideValues);
        GameObject(inject::ProvideEntries entries, GameObject& parent, ProvideValues& provideValues);

        inject::ValueProvide<GameObject> selfProvide;
        ProvideValues provideValues;
        inject::Injector injector;
        GameObjectStore& objectStore;
        GameObjectId id;

        GameObject* parent;
        GameObject* firstChild;
        GameObject* nextSibling;
        GameObject* prevSibling;

        void RemoveChild(GameObject* injector);
        void AddChild(GameObject* injector);
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
        WeakRef<Klass> getComponent(const inject::InjectorKey<Klass>& key) const {
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
}