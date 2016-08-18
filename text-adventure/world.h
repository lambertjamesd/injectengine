#pragma once

#include <unordered_map>

#include "room.h"
#include "../engine/gameobjectstore.h"
#include "../inject/injectorkey.h"

class World {
public:
    void addRoom(const engine::WeakRef<Room>& room);

    engine::WeakRef<Room> getRoom(const std::string& name) const;

    static inject::InjectorKey<World> KEY;
private:
    std::unordered_map<std::string, engine::WeakRef<Room>> rooms;
};