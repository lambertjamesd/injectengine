#include "condition.h"

BooleanCondition::BooleanCondition(const std::string& key, bool expectedValue) :
    key(key), 
    expectedValue(expectedValue) {

}

bool BooleanCondition::isTrue(const GameVariables& variables) const {
    return variables.getBoolean(key) == expectedValue;
}

BooleanCondition BooleanCondition::negate() const {
    return BooleanCondition(key, !expectedValue);
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


void AndCondition::add(const BooleanCondition& condition) {
    subConditions.push_back(condition);
}

AndCondition AndCondition::andWith(const AndCondition& other) const {
    AndCondition result(*this);
    result.andEq(other);
    return result;
}

void AndCondition::andEq(const AndCondition& other) {
    subConditions.insert(subConditions.end(), other.subConditions.begin(), other.subConditions.end());
}

Condition AndCondition::negate() const {
    Condition result;

    for (auto &subCondition : subConditions) {
        result.add(subCondition.negate());
    }

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

void Condition::add(const AndCondition& condition) {
    subConditions.push_back(condition);
}

void Condition::add(const BooleanCondition& condition) {
    AndCondition subCondition;
    subCondition.add(condition);
    subConditions.push_back(subCondition);
}

Condition Condition::andWith(const Condition& other) const {
    Condition result(*this);
    result.andEq(other);
    return result;
}

Condition Condition::andWith(const AndCondition& other) const {
    Condition result(*this);
    result.andEq(other);
    return result;
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
    }
}

void Condition::andEq(const Condition& other) {
    if (subConditions.size() == 0) {
        *this = other;
    } else {
        for (auto &thisSub : subConditions) {
            for (auto &otherSub : other.subConditions) {
                thisSub.andEq(otherSub);
            }
        }
    }
}


void Condition::andEq(const AndCondition& other) {
    if (subConditions.size() == 0) {
        add(other);
    } else {
        for (auto &thisSub : subConditions) {
            thisSub.andEq(other);
        }
    }
}

Condition Condition::negate() const {
    Condition result;

    for (auto &subCondition : subConditions) {
        result.andEq(subCondition.negate());
    }

    return result;
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