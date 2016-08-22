#pragma once

#include <string>

#include "gamestate.h"

class Action {
public:
    static Action setBoolean(const std::string& key, bool boolValue);
    static Action gotoRoom(const std::string& roomName);

    void run(GameState& state) const;

private:
    enum Type {
        SET_BOOLEAN,
        GOTO_ROOM,
    };

    Action(Type type, const std::string& key, bool boolValue);

    Type type;
    std::string key;
    bool boolValue;
};