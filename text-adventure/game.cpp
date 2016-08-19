#include "game.h"

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

        const Path* maybePath = currentRoom ? currentRoom->getPath(command) : NULL;

        if (maybePath && !maybePath->getCondition().isTrue(gameState.getVariables())) {
            maybePath = NULL;
        }

        if (command == "quit" || command == "exit") {
            running = false;
        } else if (maybePath) {
            maybePath->follow(gameState);
        } else {
            interaction.output("Did not understand ");
            interaction.output(command);
            interaction.output("\n");
        }
    }
}

inject::InjectorKey<Game> Game::KEY;