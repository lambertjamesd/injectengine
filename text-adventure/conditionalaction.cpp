#include "conditionalaction.h"

ConditionalAction::ConditionalAction(const Condition& condition, const Action& action) :
    condition(condition),
    action(action) {

}

bool ConditionalAction::tryRun(GameState& gamestate) const {
    if (condition.isTrue(gamestate.getVariables())) {
        action.run(gamestate);
        return true;
    } else {
        return false;
    }
}