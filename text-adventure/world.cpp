#include "world.h"

void World::addRoom(const engine::WeakRef<Room>& room) {
    if (room.exists()) {
        rooms.insert(std::pair<std::string, engine::WeakRef<Room>>(room.lock()->getIdentifier(), room));
    }
}

engine::WeakRef<Room> World::getRoom(const std::string& name) const {
    auto result = rooms.find(name);

    if (result != rooms.end()) {
        return result->second;
    } else {
        return engine::WeakRef<Room>();
    }
}

inject::InjectorKey<World> World::KEY;