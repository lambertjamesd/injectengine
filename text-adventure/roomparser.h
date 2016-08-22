#pragma once

#include <istream>
#include <vector>
#include <string>
#include <exception>
#include "room.h"
#include "condition.h"
#include "conditioncompiler.h"

class RoomParseError : public std::exception {
public:
    RoomParseError(const char* message);
    virtual const char* what() const throw();
private:
    const char* message;
};

class RoomParser {
public:
    void Parse(const std::string& input, RoomData& output) const;
    void Parse(std::istream& input, RoomData& output) const;
private:
    struct Scope
    {
        Scope();
        Condition condition;
        ParseState whitespace;
        std::string message;
        std::vector<Action> currentActions;
        bool isPath;
    };

    struct State {
        State();
        std::vector<Scope> scopeStack;
        bool expectIndent;
    };

    void parseLine(ParseState line, State& parseState, RoomData& output) const;
    void resolveScope(Scope& scope, RoomData& output) const;
    Scope& newScope(State& state, const Condition& condition) const;

    ConditionCompiler conditionCompiler;
};