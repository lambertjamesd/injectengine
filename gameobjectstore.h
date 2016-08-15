#pragma once

#include <unordered_set>
#include "injectorkey.h"
#include "classprovide.h"

typedef long GameObjectId;

class GameObjectStore {
public:
    GameObjectStore();
    
    bool exists(GameObjectId id) const;
    GameObjectId createId();
    void destroyId(GameObjectId id);

    static InjectorKey<GameObjectStore> KEY;
    static ClassProvide<GameObjectStore> PROVIDE;
private:
    GameObjectId currentId;
    std::unordered_set<GameObjectId> currentObjects;
};

template <typename T>
class WeakRef {
public:
    WeakRef(GameObjectStore& objectStore, GameObjectId id, T* pointer) :
        objectStore(objectStore),
        id(id),
        pointer(pointer) {

    }

    bool exists() const {
        return objectStore.exists(id);
    }
    T* lock() const {
        if (exists()) {
            return pointer;
        } else {
            return NULL;
        }
    }
private:
    GameObjectStore& objectStore;
    GameObjectId id;
    T* pointer;
};