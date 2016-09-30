#include <cstring>

#include "game.h"
#include "parsestate.h"

Game::Game(Interaction& interaction, GameState& gameState, World& world) : 
    interaction(interaction),
    gameState(gameState),
    world(world),
    running(false) {

}

void Game::run() {
    running = true;
    while (running) {
        auto currentRoom = world.getRoom(gameState.getCurrentRoom()).lock();

        if (currentRoom) {
            interaction.output(currentRoom->describe(gameState));
            currentRoom->doActions(gameState);
        } else {
            interaction.output("You are in the void of space");
        }

        interaction.output("\n: ");
        std::string command = interaction.readCommand();

        if (currentRoom) {
            command = currentRoom->rewrite(gameState.getVariables(), command);
        }

        const Path* maybePath = currentRoom ? currentRoom->getPath(command) : NULL;

        if (maybePath && !maybePath->getCondition().isTrue(gameState.getVariables())) {
            maybePath = NULL;
        }

        ParseState commandParser(command.c_str());
        commandParser.stepWord();

        if (command == "quit" || command == "exit") {
            running = false;
        } else if (maybePath) {
            maybePath->follow(gameState);
        } else if (commandParser.consume("debug"))  {
            if (commandParser.consume("get")) {
                if (gameState.getVariables().getBoolean(commandParser.toString())) {
                    interaction.output("true\n\n");
                } else {
                    interaction.output("false\n\n");
                }
            } else {
                interaction.output("Debug command not recognized\n\n");
            }
        } else {
            interaction.output("Did not understand ");
            interaction.output(command);
            interaction.output("\n");
        }
    }
}

inject::InjectorKey<Game> Game::KEY;