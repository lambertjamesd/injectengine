#include "path.h"

Path::Path(const std::string& pathName, const Condition& condition, const std::string& targetRoom) :
    pathName(pathName),
    condition(condition),
    targetRoom(targetRoom) {

}

Path::Path(const std::string& pathName, const Condition& condition, const std::string& targetRoom, const std::vector<Action>& actions) :
    pathName(pathName),
    condition(condition),
    targetRoom(targetRoom),
    actions(actions) {

}

const std::string& Path::getName() const {
    return pathName;
}

const Condition& Path::getCondition() const {
    return condition;
}

void Path::follow(GameState& gameState) const {
    gameState.setCurrentRoom(targetRoom);

    for (auto it = actions.begin(); it != actions.end(); ++it) {
        it->run(gameState);
    }
}