#include <iostream>

#include "interaction.h"

std::string Interaction::readCommand() {
    std::string result;
    std::getline(std::cin, result);
    return result;
}

void Interaction::output(const std::string& out) {
    std::cout << out;
}

inject::InjectorKey<Interaction> Interaction::KEY;