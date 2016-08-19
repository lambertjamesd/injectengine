#include "conditioncompiler.h"

#include <ctype.h>

BooleanCondition ConditionCompiler::parseBooleanCondition(ParseState& state) const {
    bool expected = !state.consume("not");
    std::string name = state.currentWord();
    state.stepWord();
    return BooleanCondition(name, expected);
}

AndCondition ConditionCompiler::parseAndCondition(ParseState& state) const {
    AndCondition result;
    do {
        result.add(parseBooleanCondition(state));
    } while (state.consume("and") && !state.eof());
    return result;
}

Condition ConditionCompiler::compile(const ParseState& stateSource) const {
    ParseState state(stateSource);
    Condition result;
    do {
        result.add(parseAndCondition(state));
    } while (state.consume("or") && !state.eof());
    return result;
}