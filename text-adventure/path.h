#pragma once

#include <vector>

#include "condition.h"
#include "action.h"
#include "gamestate.h"

class Path {
public:
    Path(const std::string& pathName, const Condition& condition, const std::string& targetRoom);
    Path(const std::string& pathName, const Condition& condition, const std::vector<Action>& actions);

    const std::string& getName() const;
    const Condition& getCondition() const;

    void follow(GameState& gameState) const;
private:
    std::string pathName;
    Condition condition;
    std::vector<Action> actions;
};