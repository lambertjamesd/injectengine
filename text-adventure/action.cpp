#include "action.h"

Action Action::setBoolean(const std::string& key, bool value) {
    return Action(key, value);
}

void Action::run(GameState& state) const {
    state.getVariables().setBoolean(key, value);
}

Action::Action(const std::string& key, bool value) :
    key(key),
    value(value) {

}