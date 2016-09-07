#pragma once

#include <string>
#include <vector>
#include "gamevariables.h"

class BooleanCondition {
public:
    BooleanCondition(const std::string& key, bool expectedValue);
    bool isTrue(const GameVariables& variables) const;

    BooleanCondition negate() const;

    std::string toString() const;
private:
    std::string key;
    bool expectedValue;
};

class Condition;

class AndCondition {
public:
    bool isTrue(const GameVariables& variables) const;
    void add(const BooleanCondition& condition);
    
    AndCondition andWith(const AndCondition& other) const;
    
    void andEq(const AndCondition& other);

    Condition negate() const;

    std::string toString() const;
private:
    std::vector<BooleanCondition> subConditions;
};

class Condition {
public:
    bool isTrue(const GameVariables& variables) const;
    void add(const AndCondition& condition);
    void add(const BooleanCondition& condition);

    Condition andWith(const Condition& other) const;
    Condition andWith(const AndCondition& other) const;
    Condition andWith(const BooleanCondition& other) const;

    void andEq(const AndCondition& other);
    void andEq(const Condition& other);

    Condition negate() const;
    
    std::string toString() const;
private:
    std::vector<AndCondition> subConditions;
};