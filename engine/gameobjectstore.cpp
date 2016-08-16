#include "gameobjectstore.h"

namespace engine {
    GameObjectStore::GameObjectStore() :
        currentId(0) {

    }

    bool GameObjectStore::exists(GameObjectId id) const {
        return currentObjects.find(id) != currentObjects.end();
    }

    GameObjectId GameObjectStore::createId() {
        GameObjectId result = currentId++;
        currentObjects.insert(result);
        return result;
    }

    void GameObjectStore::destroyId(GameObjectId id) {
        currentObjects.erase(id);
    }

    inject::InjectorKey<GameObjectStore> GameObjectStore::KEY;
    inject::ClassProvide<GameObjectStore> GameObjectStore::PROVIDE;
}