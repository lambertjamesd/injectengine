#pragma once

#include "condition.h"
#include "rewriterule.h"
#include "gamevariables.h"

class ConditionalRewriteRule {
public:
    ConditionalRewriteRule(const Condition& condition, const RewriteRule& rule);

    bool rewrite(const GameVariables& gamestate, const std::string& input, std::string& output) const;
private:
    Condition condition;
    RewriteRule rule;
};