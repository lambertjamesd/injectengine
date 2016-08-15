#include "gameobject.h"

Provide* GAME_OBJECT_PROVIE_ENTRIES[] = {

};

Provide* ROOT_GAME_OBJECT_PROVIE_ENTRIES[] = {
    &GameObjectStore::PROVIDE,
};

InjectorKey<GameObject> GameObject::KEY;

GameObject::GameObject(ProvideEntries entries) :
    selfProvide(GameObject::KEY, *this),
    injector(entries
        .extend(ROOT_GAME_OBJECT_PROVIE_ENTRIES, sizeof(ROOT_GAME_OBJECT_PROVIE_ENTRIES)/sizeof(ROOT_GAME_OBJECT_PROVIE_ENTRIES[0]))
        .extend(GAME_OBJECT_PROVIE_ENTRIES, sizeof(GAME_OBJECT_PROVIE_ENTRIES)/sizeof(GAME_OBJECT_PROVIE_ENTRIES[0]))
        .single(&selfProvide)
    ),
    objectStore(injector.get(GameObjectStore::KEY)),
    id(objectStore.createId()) {

}

GameObject::GameObject(ProvideEntries entries, GameObject& parent) :
    selfProvide(GameObject::KEY, *this),
    injector(entries
        .extend(GAME_OBJECT_PROVIE_ENTRIES, sizeof(GAME_OBJECT_PROVIE_ENTRIES)/sizeof(GAME_OBJECT_PROVIE_ENTRIES[0]))
        .single(&selfProvide)
    , &parent.injector),
    objectStore(injector.get(GameObjectStore::KEY)),
    id(objectStore.createId()) {

}

WeakRef<GameObject> GameObject::weakRef() {
    return WeakRef<GameObject>(objectStore, id, this);
}

GameObject::~GameObject() {
    objectStore.destroyId(id);
}

GameObject* GameObject::createRoot(ProvideEntries entries) {
    return new GameObject(entries);
}

GameObject* GameObject::createChild(ProvideEntries entries) {
    return new GameObject(entries, *this);
}