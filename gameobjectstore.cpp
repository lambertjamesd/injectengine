#include "gameobjectstore.h"

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

InjectorKey<GameObjectStore> GameObjectStore::KEY;
ClassProvide<GameObjectStore> GameObjectStore::PROVIDE;