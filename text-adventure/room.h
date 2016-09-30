#pragma once

#include "conditionalaction.h"
#include "description.h"
#include "gamestate.h"
#include "path.h"
#include "conditionalrewriterule.h"
#include <unordered_map>
#include <vector>

struct RoomData {
    RoomData(const std::string& identifier);

    std::string identifier;
    std::vector<Description> descriptions;
    std::vector<ConditionalAction> actions;
    std::vector<Path> paths;
    std::vector<ConditionalRewriteRule> rewriteRules;
};

class Room {
public:
    Room(const RoomData& data);

    void addDescription(const Description& description);
    void addRewriteRule(const ConditionalRewriteRule& rewriteRule);

    const std::string& getIdentifier() const;
    std::string describe(const GameState& state) const;
    void doActions(GameState& state) const;

    const Path* getPath(const std::string& name) const;
    std::string rewrite(const GameVariables& gamestate, const std::string& input) const;
private:
    std::string identifier;
    std::vector<Description> descriptions;
    std::vector<ConditionalAction> actions;
    std::unordered_map<std::string, Path> paths;
    std::vector<ConditionalRewriteRule> rewriteRules;
};