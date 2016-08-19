#pragma once

#include "condition.h"
#include "action.h"

class ConditionalAction {
public:
    ConditionalAction(const Condition& condition, const Action& action);

    bool tryRun(GameState& gamestate) const;
private:
    Condition condition;
    Action action;
};