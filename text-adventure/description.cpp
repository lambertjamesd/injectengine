#include "description.h"

Description::Description(const std::string& text) : text(text) {
    
}

std::string Description::describe() const {
    return text;
}

const Condition& Description::getCondition() const {
    return condition;
}