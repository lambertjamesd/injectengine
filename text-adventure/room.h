#pragma once

#include "description.h"
#include <vector>

struct RoomData {
    RoomData(const std::string& identifier, const std::string& description);

    std::string identifier;
    std::vector<std::string> descriptions;
};

class Room {
public:
    Room(const RoomData& data);

    void addDescription(const Description& description);

    std::string describe() const;
private:
    std::string identifier;
    std::vector<Description> descriptions;
};