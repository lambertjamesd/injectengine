#include "room.h"
#include "world.h"

RoomData::RoomData(const std::string& identifier) :
    identifier(identifier) {

}

Room::Room(const RoomData& data) :
    identifier(data.identifier),
    descriptions(data.descriptions),
    actions(actions) {
    for (auto it = data.paths.begin(); it != data.paths.end(); ++it) {
        paths.insert(std::make_pair(it->getName(), *it));
    }
}

void Room::addDescription(const Description& description) {
    descriptions.push_back(description);
}

const std::string& Room::getIdentifier() const {
    return identifier;
}

const Path* Room::getPath(const std::string& name) const {
    auto result = paths.find(name);

    if (result != paths.end()) {
        return &result->second;
    } else {
        return NULL;
    }
}

std::string Room::describe(const GameState& state) const {
    std::string result;

    for (auto it = descriptions.begin(); it != descriptions.end(); ++it) {
        if (it->getCondition().isTrue(state.getVariables())) {
            result += it->describe();
        }
    }

    if (result.length()) {
        return result;
    } else {
        return "There is nothing here";
    }
}

void Room::doActions(GameState& state) const {
    for (auto it = actions.begin(); it != actions.end(); ++it) {
        it->tryRun(state);
    }
}