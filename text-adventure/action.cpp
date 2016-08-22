#include "action.h"

Action Action::setBoolean(const std::string& key, bool boolValue) {
    return Action(SET_BOOLEAN, key, boolValue);
}

Action Action::gotoRoom(const std::string& roomName) {
    return Action(GOTO_ROOM, roomName, false);
}

void Action::run(GameState& state) const {
    switch (type) {
        case SET_BOOLEAN:
            state.getVariables().setBoolean(key, boolValue);
            break;
        case GOTO_ROOM:
            state.setCurrentRoom(key);
            break;
    }
}

Action::Action(Type type, const std::string& key, bool boolValue) :
    type(type),
    key(key),
    boolValue(boolValue) {

}