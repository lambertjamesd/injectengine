
#include <iostream>
#include <fstream>
#include <cassert>

#include "engine/gameobject.h"
#include "text-adventure/condition.h"
#include "text-adventure/conditioncompiler.h"
#include "text-adventure/game.h"
#include "text-adventure/gamestate.h"
#include "text-adventure/gamevariables.h"
#include "text-adventure/parsestate.h"
#include "text-adventure/rewriterule.h"
#include "text-adventure/room.h"
#include "text-adventure/roomparser.h"
#include "text-adventure/world.h"

using namespace engine;
using namespace inject;

void createRoom(const RoomData& roomData, GameObject& parent, const ProvideEntries& roomProvides) {
    ProvideValues data;
    data.provide(roomData);
    GameObject* result = parent.createChild(roomProvides, data);

    parent.getComponent<World>().lock()->addRoom(result->getComponent<Room>());
}

void parseGame(GameObject& game, const ProvideEntries& roomProvides) {
    std::ifstream gameStream("game.txt");
    std::string line;
    RoomParser roomParser;
    GameState& state = *game.getComponent<GameState>().lock();

    bool roomList = false;

    if (gameStream.is_open()) {
        while (gameStream.good()) {
            std::getline(gameStream, line);

            ParseState parseState(line.c_str());

            ParseState whitespace = parseState.readWhitespace();
            parseState.stepWord();

            if (!parseState.isEmpty()) {
                if (whitespace.isEmpty()) {
                    roomList = false;

                    if (parseState.consume("rooms")) {
                        roomList = true;
                    } else if (parseState.consume("start")) {
                        game.getComponent<GameState>().lock()->setCurrentRoom(parseState.toString());
                    } else if (parseState.consume("set")) {
                        std::string key = parseState.currentWord();
                        parseState.stepWord();

                        if (parseState.consume("true")) {
                            state.getVariables().setBoolean(key, true);
                        }
                    }
                } else if (roomList) {
                    RoomData roomData(parseState.currentWord());
                    parseState.stepWord();
                    std::ifstream roomStream(parseState.toString());
                    roomParser.parse(roomStream, roomData);
                    createRoom(roomData, game, roomProvides);
                }
            }

        }
        gameStream.close();
    } else {
        std::cout << "Could not load game" << std::endl;
    }
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

    rootGameObject->getComponent<GameState>().lock()->setCurrentRoom("start");

    parseGame(*rootGameObject, roomEntries);

    rootGameObject->getComponent<Game>().lock()->run();

    delete rootGameObject;
}

void testConditions() {
    GameVariables variables;
    ConditionCompiler compiler;


    {
        Condition testCondition = compiler.compile("needed-at-bridge");
        assert(!testCondition.isTrue(variables));
    }

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

void testRewriteRule() {
    {
        RewriteRule rule("foo", "bar");

        assert(rule.doesMatch("foo"));
        assert(!rule.doesMatch("fo"));
        assert(!rule.doesMatch("fooo"));
        assert(!rule.doesMatch("foo bar"));

        std::string rewrite;
        rule.rewrite("foo", rewrite);
        assert(rewrite == "bar");
    }

    {
        RewriteRule rule("shoot *", "use gun with \\0");

        std::string rewrite;
        assert(rule.rewrite("shoot enemy", rewrite));
        assert(rewrite == "use gun with enemy");
    }

    {
        RewriteRule rule("use * with *", "use \\a with \\b");

        std::string rewrite;
        assert(rule.rewrite("use cake with mouth", rewrite));
        assert(rewrite == "use cake with mouth");
        assert(rule.rewrite("use mouth with cake", rewrite));
        assert(rewrite == "use cake with mouth");
    }

    {
        RewriteRule rule("one * two * three *", "un \\0 dos \\1 tres \\2");

        std::string rewrite;
        assert(rule.rewrite("one bunny two cats three dogs", rewrite));
        assert(rewrite == "un bunny dos cats tres dogs");
        assert(!rule.doesMatch("one bunny two cats tres dogs"));
    }

    {
        RewriteRule rule("?please eat food", "eat food");

        std::string rewrite;
        assert(rule.rewrite("please eat food", rewrite));
        assert(rewrite == "eat food");
        assert(rule.rewrite("eat food", rewrite));
        assert(rewrite == "eat food");
        assert(!rule.doesMatch("please don't eat food"));
    }

    {
        RewriteRule rule("?please *", "\\0");
        std::string rewrite;
        assert(rule.rewrite("please dance", rewrite));
        assert(rewrite == "dance");
        assert(rule.rewrite("please eat the food", rewrite));
        assert(rewrite == "eat the food");
    }

    {
        RewriteRule rule("shoot *", "use gun with \\0");

        std::string rewrite;
        assert(rule.rewrite("shoot big fat cow", rewrite));
        assert(rewrite == "use gun with big fat cow");
    }

    {
        RewriteRule rule("bake * into cake", "use cake with \\0");

        std::string rewrite;
        assert(rule.rewrite("bake big fat cow into cake", rewrite));
        assert(rewrite == "use cake with big fat cow");
    }
}

int main(int argc, char const *argv[])
{
    testConditions();
    testRewriteRule();
    runGame();

    return 0;
}