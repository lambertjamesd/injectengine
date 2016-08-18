#pragma once

#include "interaction.h"
#include "gamestate.h"
#include "world.h"
#include "../inject/classprovide.h"

class Game {
public:
    Game(Interaction& interaction, GameState& gameState, World& world);

    void run();

    static inject::InjectorKey<Game> KEY;
private:
    Interaction& interaction;
    GameState& gameState;
    World& world;
    bool running;
};