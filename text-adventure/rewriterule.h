#pragma once

#include <string>
#include <vector>

class ParseState;

class RewriteRule {
public:
    RewriteRule(const std::string& inputSource, const std::string& outputSource);

    bool doesMatch(const std::string& inputText) const;
    bool rewrite(const std::string& inputText, std::string& outputText) const;
private:
    bool analyze(const std::string& inputText, std::vector<ParseState>* matches) const;
    std::string generateOutput(const std::vector<ParseState>& matches) const;

    struct InputSection {
        InputSection(const std::string& contentParam);

        std::string content;
        bool optional;

        bool isWildcard() const;
    };

    struct OutputSection {
        OutputSection(const std::string& contentParam);

        std::string content;
        int order;
        bool isAlphabetical;

        bool isReplacement() const;
    };

    std::vector<InputSection> input;
    std::vector<OutputSection> output;
    bool needsSort;
};