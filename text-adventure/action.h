#pragma once

#include <string>

#include "gamestate.h"

class Action {
public:
    static Action setBoolean(const std::string& key, bool value);

    void run(GameState& state) const;
private:
    Action(const std::string& key, bool value);

    std::string key;
    bool value;
};