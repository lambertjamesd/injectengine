#include "gamevariables.h"

bool GameVariables::getBoolean(const std::string& key) const {
    auto it = booleanStore.find(key);
    return it != booleanStore.end() && it->second;
}

void GameVariables::setBoolean(const std::string& key, bool value) {
    booleanStore[key] = value;
}