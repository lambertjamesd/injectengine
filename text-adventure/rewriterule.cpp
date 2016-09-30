#include "rewriterule.h"

#include "parsestate.h"
#include <algorithm>
#include <string.h>

RewriteRule::InputSection::InputSection(const std::string& contentParam) :
    content(contentParam),
    optional(false) {

    if (content[0] == '?') {
        optional = true;
        content = content.substr(1);
    }

    if (content == "*") {
        content.clear();
    }
}

bool RewriteRule::InputSection::isWildcard() const {
    return content.length() == 0;
}

RewriteRule::OutputSection::OutputSection(const std::string& contentParam) :
    content(contentParam),
    order(0),
    isAlphabetical(false) {
    if (content[0] == '\\') {
        if (isalpha(content[1])) {
            order = content[1] - 'a';
            isAlphabetical = true;
        } else {
            order = content[1] - '0';
        }

        content.clear();
    }
}

bool RewriteRule::OutputSection::isReplacement() const {
    return content.length() == 0;
}

RewriteRule::RewriteRule(const std::string& inputSource, const std::string& outputSource) : needsSort(false) {
    ParseState state(inputSource.c_str());
    state.stepWord();

    while (!state.eof()) {
        input.push_back(InputSection(state.currentWord()));
        state.stepWord();
    }

    ParseState outState(outputSource.c_str());
    outState.stepWord();

    while (!outState.eof()) {
        output.push_back(OutputSection(outState.currentWord()));
        needsSort = needsSort || output.back().isAlphabetical;
        outState.stepWord();
    }
}

bool RewriteRule::doesMatch(const std::string& inputText) const {
    return analyze(inputText, NULL);
}

bool RewriteRule::rewrite(const std::string& inputText, std::string& outputText) const {
    std::vector<ParseState> matches;

    if (analyze(inputText, &matches)) {
        outputText = generateOutput(matches);
        return true;
    } else {
        return false;
    }
}

bool RewriteRule::analyze(const std::string& inputText, std::vector<ParseState>* matches) const {
    ParseState state(inputText.c_str());
    ParseState wordStart;
    ParseState lastWord;
    state.stepWord();

    std::size_t index = 0;
    bool wasWildcard = false;

    while (!state.eof() && index < input.size()) {
        const InputSection& section = input.at(index);

        if (section.isWildcard()) {
            if (wasWildcard && matches) {
                matches->push_back(wordStart.wordRange(lastWord));
            }

            wasWildcard = true;
            ++index;
            wordStart = state;

            if (!section.optional) {
                lastWord = state;
                state.stepWord();
            }
        } else {
            if (section.content == state.currentWord()) {
                if (wasWildcard && matches) {
                    matches->push_back(wordStart.wordRange(lastWord));
                }
                wasWildcard = false;
                lastWord = state;
                state.stepWord();
                ++index;
            } else if (!wasWildcard) {
                if (section.optional) {
                    ++index;
                } else {
                    return false;
                }
            } else {
                lastWord = state;
                state.stepWord();
            }
        }
    }

    if (wasWildcard && matches) {
        while (!state.eof()) {
            lastWord = state;
            state.stepWord();
        }

        matches->push_back(wordStart.wordRange(lastWord));
    }

    while (index < input.size() && input[index].optional) {
        ++index;
    }

    if (index == input.size() && state.eof()) {
        return true;
    } else {
        return false;
    }
}

std::string RewriteRule::generateOutput(const std::vector<ParseState>& matches) const {
    std::vector<ParseState> alphaMatches;
    std::string outputText;

    if (needsSort) {
        alphaMatches = matches;
        std::sort(alphaMatches.begin(), alphaMatches.end());
    }

    outputText.clear();

    for (auto& out: output) {
        if (outputText.length()) {
            outputText += ' ';
        }

        if (out.isReplacement()) {
            if (out.order < matches.size()) {
                if (out.isAlphabetical) {
                    outputText += alphaMatches[out.order].toString();
                } else {
                    outputText += matches[out.order].toString();
                }
            } else {
                outputText += "[overflow]";
            }
        } else {
            outputText += out.content;
        }
    }

    return outputText;
}