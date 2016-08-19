#pragma once

#include <string>
#include "condition.h"

class Description {
public:
    Description(const std::string& text);
    Description(const std::string& text, const Condition& condition);

    std::string describe() const;
    const Condition& getCondition() const;
private:
    std::string text;
    Condition condition;
};