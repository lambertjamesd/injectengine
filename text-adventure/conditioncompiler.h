#pragma once

#include "condition.h"
#include "parsestate.h"

class ConditionCompiler {
public:
    Condition compile(const ParseState& state) const;
private:

    BooleanCondition parseBooleanCondition(ParseState& stateSource) const;
    AndCondition parseAndCondition(ParseState& stateSource) const;
};