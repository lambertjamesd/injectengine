#pragma once

#include <string>
#include "../inject/injectorkey.h"

class Interaction {
public:
    std::string readCommand();
    void output(const std::string& out);

    static inject::InjectorKey<Interaction> KEY;
private:
};