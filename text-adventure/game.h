#pragma once

#include "interaction.h"
#include "../inject/classprovide.h"

class Game {
public:
    Game(Interaction& interaction);

    void run();

    static inject::InjectorKey<Game> KEY;
private:
    Interaction& interaction;
    bool running;
};