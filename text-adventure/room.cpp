#include "room.h"

RoomData::RoomData(const std::string& identifier, const std::string& description) :
    identifier(identifier) {
    descriptions.push_back(description);
}

Room::Room(const RoomData& data) :
    identifier(data.identifier) {
    for (auto it = data.descriptions.begin(); it != data.descriptions.end(); ++it) {
        descriptions.push_back(Description(*it));
    }
}

void Room::addDescription(const Description& description) {
    descriptions.push_back(description);
}

std::string Room::describe() const {
    if (descriptions.size() == 0) {
        return "There is nothing here";
    } else {
        std::string result;

        for (auto it = descriptions.begin(); it != descriptions.end(); ++it) {
            result += it->describe();
        }

        return result;
    }
}