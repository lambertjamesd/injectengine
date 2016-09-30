#include "conditionalrewriterule.h"

ConditionalRewriteRule::ConditionalRewriteRule(const Condition& condition, const RewriteRule& rule) :
    condition(condition),
    rule(rule) {

}

bool ConditionalRewriteRule::rewrite(const GameVariables& gamestate, const std::string& input, std::string& output) const {
    if (condition.isTrue(gamestate)) {
        return rule.rewrite(input, output);
    } else {
        return false;
    }
}
