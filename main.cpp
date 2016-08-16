
#include <iostream>

#include "text-adventure/game.h"
#include "text-adventure/room.h"
#include "engine/gameobject.h"

using namespace engine;
using namespace inject;

void createRoom(const RoomData& roomData, GameObject& parent, const ProvideEntries& roomProvides) {
    ProvideValues data;
    data.provide(roomData);
    GameObject* result = parent.createChild(roomProvides, data);
    result->getComponent<Room>();
}

void runGame() {
    ClassProvide<Game, Interaction> gameProvide(Interaction::KEY);
    ClassProvide<Interaction> interactionProvide;

    Provide* rootProvides[] = {
        &gameProvide,
        &interactionProvide,
    };

    ClassProvide<Room, RoomData> roomProvide(InjectorKey<RoomData>::keyForClass());

    Provide* roomProvides[] = {
        &roomProvide,
    };

    GameObject* rootGameObject = GameObject::createRoot(
        ProvideEntries(rootProvides, sizeof(rootProvides) / sizeof(rootProvides[0]))
    );

    ProvideEntries roomEntries(roomProvides, sizeof(roomProvides) / sizeof(roomProvides[0]));

    createRoom(RoomData("start", "You see a foo bar"), *rootGameObject, roomEntries);

    rootGameObject->getComponent<Game>().lock()->run();

    delete rootGameObject;
}

int main(int argc, char const *argv[])
{
    runGame();

    return 0;
}