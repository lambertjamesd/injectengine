#include "roomparser.h"
#include "parsestate.h"

State::State() : 
    expectIndent(false) {
    scopeStack.push_back(Scope());
}

void RoomParser::Parse(const std::string& input, RoomData& output) const {
    State parseState;
    ParseState stream(input.c_str());

    while (!stream.eof()) {
        parseLine(stream.readLine(), parseState, output);
    }
}

void RoomParser::parseLine(ParseState line, State& parseState, RoomData& output) {
    ParseState whitespace = line.readWhitespace();
    const Scope& scope = parseState.scopeStack.back();
    const ParseState& lastWhitespace = scope.whitespace;

    bool noIndent = whitespace.isSubsetOf(lastWhitespace);
    bool noRetract = line.isEmpty() || lastWhitespace.isSubsetOf(whitespace);

    if (noIndent && noRetract) {

    } else if (!noIndent && noRetract) {
        if (!parseState.expectIndent) {
            throw RoomParseError("Indentation not expected");
        } else {
            // Handle indentation
        }
    } else if (noIndent && !noRetract) {
        while (scopeStack.size() && scopeStack.back().whitespace != whitespace) {
            scopeStack.pop_back();
        }

        if (scopeStack.size() == 0) {
            throw RoomParseError("Bad indentation");
        }
    }
}