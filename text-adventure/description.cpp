#include "description.h"

Description::Description(const std::string& text) : text(text) {
    
}

Description::Description(const std::string& text, const Condition& condition) : 
    text(text),
    condition(condition) {
    
}

std::string Description::describe() const {
    return text;
}

const Condition& Description::getCondition() const {
    return condition;
}