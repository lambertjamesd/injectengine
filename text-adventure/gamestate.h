#pragma once

#include <string>
#include "../inject/injectorkey.h"
#include "gamevariables.h"

class GameState {
public:
    void setCurrentRoom(const std::string& room);
    const std::string getCurrentRoom() const;
    const GameVariables& getVariables() const;

    static inject::InjectorKey<GameState> KEY;
private:
    std::string currentRoom;
    GameVariables variables;
};