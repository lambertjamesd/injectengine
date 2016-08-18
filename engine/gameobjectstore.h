#pragma once

#include <unordered_set>
#include "../inject/injectorkey.h"
#include "../inject/classprovide.h"

namespace engine {
    typedef long GameObjectId;

    class GameObjectStore {
    public:
        GameObjectStore();
        
        bool exists(GameObjectId id) const;
        GameObjectId createId();
        void destroyId(GameObjectId id);

        static inject::InjectorKey<GameObjectStore> KEY;
        static inject::ClassProvide<GameObjectStore> PROVIDE;
    private:
        GameObjectId currentId;
        std::unordered_set<GameObjectId> currentObjects;
    };

    template <typename T>
    class WeakRef {
    public:
        WeakRef() :
            objectStore(0),
            id(0),
            pointer(0) {

        }

        WeakRef(GameObjectStore& objectStore, GameObjectId id, T* pointer) :
            objectStore(&objectStore),
            id(id),
            pointer(pointer) {

        }

        bool exists() const {
            return objectStore && objectStore->exists(id);
        }
        
        T* lock() const {
            if (exists()) {
                return pointer;
            } else {
                return NULL;
            }
        }
    private:
        GameObjectStore* objectStore;
        GameObjectId id;
        T* pointer;
    };
}