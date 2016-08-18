#include "condition.h"

BooleanCondition::BooleanCondition(const std::string& key, bool expectedValue) :
    key(key), 
    expectedValue(expectedValue) {

}

bool BooleanCondition::isTrue(const GameVariables& variables) const {
    return variables.getBoolean(key) == expectedValue;
}

bool AndCondition::isTrue(const GameVariables& variables) const {
    for (auto it = subConditions.begin(); it != subConditions.end(); ++it) {
        if (!it->isTrue(variables)) {
            return false;
        }
    }

    return true;
}

void AndCondition::add(const BooleanCondition& conditoin) {
    subConditions.push_back(conditoin);
}

AndCondition AndCondition::andWith(const AndCondition& other) const {
    AndCondition result(*this);
    result.subConditions.insert(result.subConditions.end(), other.subConditions.begin(), other.subConditions.end());
    return result;
}

bool Condition::isTrue(const GameVariables& variables) const {
    for (auto it = subConditions.begin(); it != subConditions.end(); ++it) {
        if (it->isTrue(variables)) {
            return true;
        }
    }

    return subConditions.size() == 0;
}

void Condition::add(const AndCondition& conditoin) {
    subConditions.push_back(conditoin);
}

Condition Condition::andWith(const Condition& other) const {
    Condition result;

    for (auto thisSub = subConditions.begin(); thisSub != subConditions.end(); ++thisSub) {
        for (auto otherSub = other.subConditions.begin(); otherSub != other.subConditions.end(); ++otherSub) {
            result.subConditions.push_back(thisSub->andWith(*otherSub));
        }
    }

    return result;
}