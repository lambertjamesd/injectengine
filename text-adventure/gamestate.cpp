#include "gamestate.h"

void GameState::setCurrentRoom(const std::string& room) {
    currentRoom = room;
}

const std::string GameState::getCurrentRoom() const {
    return currentRoom;
}

const GameVariables& GameState::getVariables() const {
    return variables;
}

GameVariables& GameState::getVariables() {
    return variables;
}

inject::InjectorKey<GameState> GameState::KEY;