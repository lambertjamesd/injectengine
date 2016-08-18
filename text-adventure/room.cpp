#include "room.h"
#include "world.h"

RoomData::RoomData(const std::string& identifier, const std::string& description) :
    identifier(identifier) {
    descriptions.push_back(description);
}

Room::Room(const RoomData& data) :
    identifier(data.identifier),
    paths(data.paths) {
    for (auto it = data.descriptions.begin(); it != data.descriptions.end(); ++it) {
        descriptions.push_back(Description(*it));
    }
}

void Room::addDescription(const Description& description) {
    descriptions.push_back(description);
}

const std::string& Room::getIdentifier() const {
    return identifier;
}

const std::string* Room::getPath(const std::string& name) const {
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