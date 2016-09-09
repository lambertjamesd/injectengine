#include "parsestate.h"

#include <string.h>

ParseState::ParseState() :
    wordStart(NULL),
    wordEnd(NULL),
    streamEnd(NULL) {

}

ParseState::ParseState(const char* start) : 
    wordStart(start),
    wordEnd(start),
    streamEnd(start + strlen(start)) {

}

ParseState::ParseState(const char* start, const char* end) : 
    wordStart(start),
    wordEnd(start),
    streamEnd(end) {
        
}

void ParseState::ensureWord() {
    if (wordStart == wordEnd) {
        stepWord();
    }
}

void ParseState::stepWord() {
    while (wordEnd != streamEnd && *wordEnd && isspace(*wordEnd)) {
        ++wordEnd;
    }

    wordStart = wordEnd;

    while (wordEnd != streamEnd && *wordEnd && !isspace(*wordEnd)) {
        ++wordEnd;
    }
}

std::string ParseState::currentWord() const {
    return std::string(wordStart, wordEnd);
}

std::string ParseState::toString() const {
    return std::string(wordStart, streamEnd);
}

bool ParseState::isNext(const char* word) const {
    return strncmp(wordStart, word, wordEnd - wordStart) == 0;
}

bool ParseState::consume(const char* word) {
    if (isNext(word)) {
        stepWord();
        return true;
    } else {
        return false;
    }
}

bool ParseState::consumePartial(const char* word) {
    const char* startStep = wordStart;

    while (*word) {
        if (*word != *startStep) {
            return false;
        }

        ++word;
        ++startStep;
    }

    if (startStep > streamEnd) {
        return false;
    }

    wordStart = startStep;
    wordEnd = std::max(wordStart, wordEnd);
    return true;
}

bool ParseState::eof() const {
    return wordStart == streamEnd || *wordStart == '\0';
}

ParseState ParseState::readLine() {
    const char* begin = wordStart;

    while (!eof()) {
        if (*wordStart == '\r' || *wordStart == '\n') {
            const char* end = wordStart;

            if ((wordStart[1] == '\r' || wordStart[1] == '\n') 
                && wordStart[1] != wordStart[0] 
                && wordStart + 1 != streamEnd ) {
                wordStart += 2;
            } else {
                wordStart += 1;
            }

            wordEnd = wordStart;

            return ParseState(begin, end);
        }

        ++wordStart;
    }

    wordEnd = wordStart;

    return ParseState(begin, wordStart);
}

ParseState ParseState::readWhitespace() {
    const char* begin = wordStart;

    while (!eof()) {
        if (!isspace(*wordStart)) {
            break;
        }
        ++wordStart;
    }

    wordEnd = wordStart;

    return ParseState(begin, wordStart);
}

std::size_t ParseState::length() const {
    return static_cast<std::size_t>(streamEnd - wordStart);
}

const char* ParseState::begin() const {
    return wordStart;
}

bool ParseState::isSubsetOf(const ParseState& other) const {
    if (length() > other.length()) {
        return false;
    } else {
        return strncmp(wordStart, other.wordStart, length()) == 0;
    }
}

bool ParseState::isEmpty() const {
    return wordStart == streamEnd || *wordStart == '\0';   
}

void ParseState::clear() {
    wordStart = streamEnd;
}

bool ParseState::operator ==(const ParseState& other) const {
    return (length() == other.length()) && (length() == 0 || strncmp(wordStart, other.wordStart, length()) == 0);
}

bool ParseState::operator !=(const ParseState& other) const {
    return !(other == *this);
}