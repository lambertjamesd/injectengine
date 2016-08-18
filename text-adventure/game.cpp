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
        } else {
            interaction.output("You are in the void of space");
        }

        interaction.output("\n: ");
        std::string command = interaction.readCommand();

        if (command == "quit" || command == "exit") {
            running = false;
        } else if (currentRoom && currentRoom->getPath(command)) {
            gameState.setCurrentRoom(*currentRoom->getPath(command));
        } else {
            interaction.output("Did not understand ");
            interaction.output(command);
            interaction.output("\n");
        }
    }
}

inject::InjectorKey<Game> Game::KEY;