#pragma once

#include <unordered_map>
#include <string>

class GameVariables {
public:
    bool getBoolean(const std::string& key) const;
    void setBoolean(const std::string& key, bool value);
private:
    std::unordered_map<std::string, bool> booleanStore;
};