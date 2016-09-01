#include "condition.h"

BooleanCondition::BooleanCondition(const std::string& key, bool expectedValue) :
    key(key), 
    expectedValue(expectedValue) {

}

bool BooleanCondition::isTrue(const GameVariables& variables) const {
    return variables.getBoolean(key) == expectedValue;
}

std::string BooleanCondition::toString() const {
    if (expectedValue) {
        return key;
    } else {
        return "not " + key;
    }
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

std::string AndCondition::toString() const {
    std::string result;

    for (auto it = subConditions.begin(); it != subConditions.end(); ++it) {
        if (result.length()) {
            result += " and ";
        }

        result += it->toString();
    }

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

Condition Condition::andWith(const AndCondition& other) const {
    if (subConditions.size() == 0) {
        Condition result;
        result.add(other);
        return result;
    } else {
        Condition result;

        for (auto thisSub = subConditions.begin(); thisSub != subConditions.end(); ++thisSub) {
            result.subConditions.push_back(thisSub->andWith(other));
        }

        return result;
    }
}

Condition Condition::andWith(const BooleanCondition& other) const {
    if (subConditions.size() == 0) {
        Condition result;
        AndCondition andPart;
        andPart.add(other);
        result.add(andPart);
        return result;
    } else {
        Condition result(*this);

        for (auto resultSub = result.subConditions.begin(); resultSub != result.subConditions.end(); ++resultSub) {
            resultSub->add(other);
        }

        return result;
    }
}

std::string Condition::toString() const {
    std::string result;

    for (auto it = subConditions.begin(); it != subConditions.end(); ++it) {
        if (result.length()) {
            result += " or ";
        }

        result += it->toString();
    }

    return result;
}