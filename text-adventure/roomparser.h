#pragma once

#include <vector>
#include <string>
#include <exception>
#include "room.h"
#include "condition.h"

class RoomParseError : public std::exception {
public:
    RoomParseError(const char* what);
    virtual const char* what() const throw();
private:
    const char* what;
};

class RoomParser {
public:
    void Parse(const std::string& input, RoomData& output) const;
private:
    struct Scope
    {
        Condition condition;
        ParseState whitespace;
    };

    struct State {
        State();
        std::vector<Scope> scopeStack;
        bool expectIndent;
    };

    void parseLine(ParseState line, State& parseState, RoomData& output);
};