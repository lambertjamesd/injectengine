#pragma once

#include <string>
#include <vector>
#include "gamevariables.h"

class BooleanCondition {
public:
    BooleanCondition(const std::string& key, bool expectedValue);
    bool isTrue(const GameVariables& variables) const;
private:
    std::string key;
    bool expectedValue;
};

class AndCondition {
public:
    bool isTrue(const GameVariables& variables) const;
    void add(const BooleanCondition& conditoin);
    
    AndCondition andWith(const AndCondition& other) const;
private:
    std::vector<BooleanCondition> subConditions;
};

class Condition {
public:
    bool isTrue(const GameVariables& variables) const;
    void add(const AndCondition& conditoin);

    Condition andWith(const Condition& other) const;
private:
    std::vector<AndCondition> subConditions;
};