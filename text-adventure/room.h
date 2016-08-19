#pragma once

#include "description.h"
#include "gamestate.h"
#include "path.h"
#include <vector>
#include <unordered_map>
#include "conditionalaction.h"

struct RoomData {
    RoomData(const std::string& identifier);

    std::string identifier;
    std::vector<Description> descriptions;
    std::vector<ConditionalAction> actions;
    std::vector<Path> paths;
};

class Room {
public:
    Room(const RoomData& data);

    void addDescription(const Description& description);

    const std::string& getIdentifier() const;
    std::string describe(const GameState& state) const;
    void doActions(GameState& state) const;

    const Path* getPath(const std::string& name) const;
private:
    std::string identifier;
    std::vector<Description> descriptions;
    std::vector<ConditionalAction> actions;
    std::unordered_map<std::string, Path> paths;
};