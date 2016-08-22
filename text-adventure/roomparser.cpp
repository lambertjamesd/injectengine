#include "roomparser.h"
#include "parsestate.h"

#include <iostream>

RoomParseError::RoomParseError(const char* message) :
    message(message) {

}

const char* RoomParseError::what() const throw() {
    return message;
}

RoomParser::Scope::Scope() : 
    isPath(false) {

}

RoomParser::State::State() : 
    expectIndent(false) {
    scopeStack.push_back(Scope());
}

void RoomParser::parse(std::istream& input, RoomData& output) const {
    State parseState;
    std::string line;

    while (!input.eof()) {
        std::getline(input, line);
        parseLine(ParseState(line.c_str()), parseState, output);
    }

    while (parseState.scopeStack.size()) {
        resolveScope(parseState.scopeStack.back(), output);
        parseState.scopeStack.pop_back();
    }
}

void RoomParser::parse(const std::string& input, RoomData& output) const {
    State parseState;
    ParseState stream(input.c_str());

    while (!stream.eof()) {
        parseLine(stream.readLine(), parseState, output);
    }

    while (parseState.scopeStack.size()) {
        resolveScope(parseState.scopeStack.back(), output);
        parseState.scopeStack.pop_back();
    }
}

void RoomParser::parseLine(ParseState line, State& parseState, RoomData& output) const {
    ParseState whitespace = line.readWhitespace();
    const ParseState& lastWhitespace = parseState.scopeStack.back().whitespace;

    bool noIndent = whitespace.isSubsetOf(lastWhitespace);
    bool noRetract = line.isEmpty() || lastWhitespace.isSubsetOf(whitespace);

    if (noIndent && !noRetract) {
        while (parseState.scopeStack.size() && parseState.scopeStack.back().whitespace != whitespace) {
            resolveScope(parseState.scopeStack.back(), output);
            parseState.scopeStack.pop_back();
        }

        if (parseState.scopeStack.size() == 0) {
            throw RoomParseError("Bad indentation");
        }
    } else if (!noIndent && noRetract) {
        if (!parseState.expectIndent) {
            throw RoomParseError("Indentation not expected");
        } else {
            parseState.scopeStack.back().whitespace = whitespace;
            parseState.expectIndent = false;
        }
    } else if (!noIndent && !noRetract) {
        throw RoomParseError("whitepsace mismatch");
    }

    Scope& scope = parseState.scopeStack.back();

    if (line.consumePartial("*")) {
        line.stepWord();

        if (line.consume("if")) {
            newScope(parseState, conditionCompiler.compile(line), output);
        } else if (line.consume("action")) {
            Scope& actionScope = newScope(parseState, Condition(), output);
            actionScope.message = line.toString();
            actionScope.isPath = true;
        } else if (line.consume("goto")) {
            scope.currentActions.push_back(Action::gotoRoom(line.toString()));
        } else if (line.consume("set")) {
            std::string key = line.currentWord();
            line.stepWord();
            
            if (line.consume("true")) {
                scope.currentActions.push_back(Action::setBoolean(key, true));
            } else if (line.consume("false")) {
                scope.currentActions.push_back(Action::setBoolean(key, false));
            } else {
                throw RoomParseError("Invalid value for set");
            }
        } else {
            throw RoomParseError("Unrecognized command");
        }
    } else {
        if (scope.message.length() && !line.isEmpty()) {
            scope.message += '\n';
        }

        line.consumePartial(">");

        scope.message.append(line.begin(), line.length());
    }
}

void RoomParser::resolveScope(Scope& scope, RoomData& output) const {
    if (scope.isPath) {
        output.paths.push_back(Path(scope.message, scope.condition, scope.currentActions));
    } else {
        for (auto it = scope.currentActions.begin(); it != scope.currentActions.end(); ++it) {
            output.actions.push_back(ConditionalAction(scope.condition, *it));
        }
        output.descriptions.push_back(Description(scope.message, scope.condition));
    }

    scope.message.clear();
    scope.currentActions.clear();
}

RoomParser::Scope& RoomParser::newScope(State& parseState, const Condition& condition, RoomData& output) const {
    Scope& scope = parseState.scopeStack.back();

    if (scope.isPath) {
        throw RoomParseError("Cannot add new scope to path");
    } else {
        resolveScope(scope, output);

        Scope newScope;
        newScope.whitespace = scope.whitespace;
        newScope.condition = scope.condition.andWith(condition);
        parseState.scopeStack.push_back(newScope);
        parseState.expectIndent = true;

        return parseState.scopeStack.back();
    }
}