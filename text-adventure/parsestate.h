#pragma once

#include <string>

class ParseState {
public:
    ParseState();
    ParseState(const char* start);
    ParseState(const char* start, const char* end);

    void stepWord();
    std::string currentWord() const;
    std::string toString() const;
    bool isNext(const char* word) const;
    bool consume(const char* word);
    bool eof() const;

    ParseState readLine();
    ParseState readWhitespace();

    std::size_t length() const;
    bool isSubsetOf(const ParseState& other) const;
    bool isEmpty() const;

    bool operator ==(const ParseState& other) const;
    bool operator !=(const ParseState& other) const;
private:
    const char* wordStart;
    const char* wordEnd;
    const char* streamEnd;
};