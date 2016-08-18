#pragma once

#include "description.h"
#include "gamestate.h"
#include <vector>
#include <unordered_map>

struct RoomData {
    RoomData(const std::string& identifier, const std::string& description);

    std::string identifier;
    std::vector<std::string> descriptions;
    std::unordered_map<std::string, std::string> paths;
};

class Room {
public:
    Room(const RoomData& data);

    void addDescription(const Description& description);

    const std::string& getIdentifier() const;
    std::string describe(const GameState& state) const;

    const std::string* getPath(const std::string& name) const;
private:
    std::string identifier;
    std::vector<Description> descriptions;
    std::unordered_map<std::string, std::string> paths;
};