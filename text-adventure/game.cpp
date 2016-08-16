#include "game.h"

Game::Game(Interaction& interaction) : 
    interaction(interaction),
    running(false) {

}

void Game::run() {
    running = true;
    while (running) {
        std::string command = interaction.readCommand();

        if (command == "quit" || command == "exit") {
            running = false;
        } else {
            interaction.output("You typed: ");
            interaction.output(command);
            interaction.output("\n");
        }
    }
}

inject::InjectorKey<Game> Game::KEY;