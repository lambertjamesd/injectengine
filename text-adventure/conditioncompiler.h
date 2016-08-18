#pragma once

#include "condition.h"

class ConditionCompiler {
public:
    Condition compile(const std::string& condition) const;
private:
    struct State {
        State(const char* source);

        const char* source;
        std::size_t wordStart;
        std::size_t wordEnd;

        void stepWord();
        std::string currentWord() const;
        bool isNext(const char* word) const;
        bool consume(const char* word);
        bool eof() const;
    };

    BooleanCondition parseBooleanCondition(State& state) const;
    AndCondition parseAndCondition(State& state) const;
};