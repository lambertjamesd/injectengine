#pragma once

#include <string>

class ParseState {
public:
    ParseState();
    ParseState(const char* start);
    ParseState(const char* start, const char* end);

    void ensureWord();
    void stepWord();
    std::string currentWord() const;
    std::string toString() const;
    bool isNext(const char* word) const;
    bool consume(const char* word);
    bool consumePartial(const char* word);
    bool eof() const;

    ParseState readLine();
    ParseState readWhitespace();
    ParseState wordRange(const ParseState& next) const;

    ParseState split(const char* word);

    std::size_t length() const;
    const char* begin() const;
    bool isSubsetOf(const ParseState& other) const;
    bool isEmpty() const;

    void clear();

    bool operator ==(const ParseState& other) const;
    bool operator !=(const ParseState& other) const;

    bool operator <(const ParseState& other) const;
private:
    const char* wordStart;
    const char* wordEnd;
    const char* streamEnd;
};