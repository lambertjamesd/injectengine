#include "gameobject.h"

namespace engine {
    inject::Provide* GAME_OBJECT_PROVIE_ENTRIES[] = {

    };

    inject::Provide* ROOT_GAME_OBJECT_PROVIE_ENTRIES[] = {
        &GameObjectStore::PROVIDE,
    };

    inject::InjectorKey<GameObject> GameObject::KEY;

    GameObject::GameObject(inject::ProvideEntries entries, ProvideValues& provideValues) :
        selfProvide(GameObject::KEY, *this),
        provideValues(provideValues),
        injector(entries
            .extend(ROOT_GAME_OBJECT_PROVIE_ENTRIES, sizeof(ROOT_GAME_OBJECT_PROVIE_ENTRIES)/sizeof(ROOT_GAME_OBJECT_PROVIE_ENTRIES[0]))
            .extend(GAME_OBJECT_PROVIE_ENTRIES, sizeof(GAME_OBJECT_PROVIE_ENTRIES)/sizeof(GAME_OBJECT_PROVIE_ENTRIES[0]))
            .single(&selfProvide)
            .join(this->provideValues.provideEntries())
        ),
        objectStore(injector.get(GameObjectStore::KEY)),
        id(objectStore.createId()),
        parent(0),
        firstChild(0),
        nextSibling(0),
        prevSibling(0) {

    }

    GameObject::GameObject(inject::ProvideEntries entries, GameObject& parent, ProvideValues& provideValues) :
        selfProvide(GameObject::KEY, *this),
        provideValues(provideValues),
        injector(entries
            .extend(GAME_OBJECT_PROVIE_ENTRIES, sizeof(GAME_OBJECT_PROVIE_ENTRIES)/sizeof(GAME_OBJECT_PROVIE_ENTRIES[0]))
            .single(&selfProvide)
            .join(this->provideValues.provideEntries())
        , &parent.injector),
        objectStore(injector.get(GameObjectStore::KEY)),
        id(objectStore.createId()),
        parent(0),
        firstChild(0),
        nextSibling(0),
        prevSibling(0) {
        parent.AddChild(this);
    }

    WeakRef<GameObject> GameObject::weakRef() {
        return WeakRef<GameObject>(objectStore, id, this);
    }

    GameObject::~GameObject() {
        while (firstChild) {
            delete firstChild;
        }

        if (parent) {
            parent->RemoveChild(this);
        }

        objectStore.destroyId(id);
    }

    GameObject* GameObject::createRoot(inject::ProvideEntries entries) {
        ProvideValues provideValues;
        return new GameObject(entries, provideValues);
    }

    GameObject* GameObject::createRoot(inject::ProvideEntries entries, ProvideValues& provideValues) {
        return new GameObject(entries, provideValues);
    }

    GameObject* GameObject::createChild(inject::ProvideEntries entries) {
        ProvideValues provideValues;
        return new GameObject(entries, *this, provideValues);
    }

    GameObject* GameObject::createChild(inject::ProvideEntries entries, ProvideValues& provideValues) {
        return new GameObject(entries, *this, provideValues);
    }

    void GameObject::RemoveChild(GameObject* gameObject) {
        GameObject* next = gameObject->nextSibling;
        GameObject* prev = gameObject->prevSibling;

        if (next) {
            next->prevSibling = prev;
        }

        if (prev) {
            prev->nextSibling = next;
        }

        if (gameObject == firstChild) {
            firstChild = gameObject->nextSibling;
        }

        gameObject->parent = 0;
        gameObject->firstChild = 0;
        gameObject->nextSibling = 0;
        gameObject->prevSibling = 0;
    }

    void GameObject::AddChild(GameObject* gameObject) {
        gameObject->parent = this;
        gameObject->nextSibling = firstChild;

        if (firstChild) {
            firstChild->prevSibling = gameObject;
        }

        firstChild = gameObject;
    }
}