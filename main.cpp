
#include <iostream>
#include <cassert>

#include "engine/gameobject.h"
#include "text-adventure/game.h"
#include "text-adventure/room.h"
#include "text-adventure/world.h"
#include "text-adventure/gamestate.h"
#include "text-adventure/condition.h"
#include "text-adventure/conditioncompiler.h"
#include "text-adventure/gamevariables.h"
#include "text-adventure/roomparser.h"

using namespace engine;
using namespace inject;

void createRoom(const RoomData& roomData, GameObject& parent, const ProvideEntries& roomProvides) {
    ProvideValues data;
    data.provide(roomData);
    GameObject* result = parent.createChild(roomProvides, data);

    parent.getComponent<World>().lock()->addRoom(result->getComponent<Room>());
}

void runGame() {
    ClassProvide<Game, Interaction, GameState, World> gameProvide(Interaction::KEY, GameState::KEY, World::KEY);
    ClassProvide<Interaction> interactionProvide;
    ClassProvide<World> worldProvide;
    ClassProvide<GameState> gameStateProvide;

    ConditionCompiler compiler;
    RoomParser roomParser;

    Provide* rootProvides[] = {
        &gameProvide,
        &interactionProvide,
        &worldProvide,
        &gameStateProvide,
    };

    ClassProvide<Room, RoomData> roomProvide(InjectorKey<RoomData>::keyForClass());

    Provide* roomProvides[] = {
        &roomProvide,
    };

    GameObject* rootGameObject = GameObject::createRoot(
        ProvideEntries(rootProvides, sizeof(rootProvides) / sizeof(rootProvides[0]))
    );

    ProvideEntries roomEntries(roomProvides, sizeof(roomProvides) / sizeof(roomProvides[0]));

    RoomData roomData("start");

    roomData.descriptions.push_back(Description("You see a foo bar", compiler.compile("a")));
    roomData.paths.push_back(Path("eject", Condition(), "space"));

    createRoom(roomData, *rootGameObject, roomEntries);

    RoomData space("space");

    roomParser.Parse(
        "You are in space and will die\n"
        "*action foo\n"
        "   *goto start\n"
        "   *set a true"
    , space);

    createRoom(space, *rootGameObject, roomEntries);

    rootGameObject->getComponent<GameState>().lock()->setCurrentRoom("start");

    rootGameObject->getComponent<Game>().lock()->run();

    delete rootGameObject;
}

void testConditions() {
    GameVariables variables;
    ConditionCompiler compiler;

    {
        Condition testCondition = compiler.compile("a");
        assert(!testCondition.isTrue(variables));
    }

    variables.setBoolean("a", true);

    {
        Condition testCondition = compiler.compile("a");
        assert(testCondition.isTrue(variables));
    }

    variables.setBoolean("b", false);

    {
        Condition testCondition = compiler.compile("a and b");
        assert(!testCondition.isTrue(variables));
        variables.setBoolean("b", true);
        assert(testCondition.isTrue(variables));
    }

    {
        Condition testCondition = compiler.compile("not a");
        assert(!testCondition.isTrue(variables));
        variables.setBoolean("a", false);
        assert(testCondition.isTrue(variables));
    }

    {
        Condition testCondition = compiler.compile("b and not a");
        assert(testCondition.isTrue(variables));
    }

    {
        Condition testCondition = compiler.compile("a or b");
        assert(testCondition.isTrue(variables));
        variables.setBoolean("b", false);
        assert(!testCondition.isTrue(variables));
    }

    variables.setBoolean("c", true);

    {
        Condition testCondition = compiler.compile("a and b or c");
        assert(testCondition.isTrue(variables));
        variables.setBoolean("c", false);
        assert(!testCondition.isTrue(variables));
        variables.setBoolean("a", true);
        assert(!testCondition.isTrue(variables));
        variables.setBoolean("b", true);
        assert(testCondition.isTrue(variables));
    }
}

int main(int argc, char const *argv[])
{
    testConditions();
    runGame();

    return 0;
}