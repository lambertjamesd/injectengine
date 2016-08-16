#pragma once

#include <string>

class Description {
public:
    Description(const std::string& text);

    std::string describe() const;
private:
    std::string text;
};