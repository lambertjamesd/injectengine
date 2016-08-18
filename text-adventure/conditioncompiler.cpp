#include "conditioncompiler.h"

#include <ctype.h>
#include <string.h>

ConditionCompiler::State::State(const char* source) : 
    source(source),
    wordStart(0),
    wordEnd(0) {
    stepWord();
}

void ConditionCompiler::State::stepWord() {
    while (source[wordEnd] && isspace(source[wordEnd])) {
        ++wordEnd;
    }

    wordStart = wordEnd;

    while (source[wordEnd] && !isspace(source[wordEnd])) {
        ++wordEnd;
    }
}

std::string ConditionCompiler::State::currentWord() const {
    return std::string(source + wordStart, source + wordEnd);
}

bool ConditionCompiler::State::isNext(const char* word) const {
    return strncmp(source + wordStart, word, wordEnd - wordStart) == 0;
}

bool ConditionCompiler::State::consume(const char* word) {
    if (isNext(word)) {
        stepWord();
        return true;
    } else {
        return false;
    }
}

bool ConditionCompiler::State::eof() const {
    return !source[wordStart];
}

BooleanCondition ConditionCompiler::parseBooleanCondition(State& state) const {
    bool expected = !state.consume("not");
    std::string name = state.currentWord();
    state.stepWord();
    return BooleanCondition(name, expected);
}

AndCondition ConditionCompiler::parseAndCondition(State& state) const {
    AndCondition result;
    do {
        result.add(parseBooleanCondition(state));
    } while (state.consume("and") && !state.eof());
    return result;
}

Condition ConditionCompiler::compile(const std::string& condition) const {
    State state(condition.c_str());
    Condition result;
    do {
        result.add(parseAndCondition(state));
    } while (state.consume("or") && !state.eof());
    return result;
}