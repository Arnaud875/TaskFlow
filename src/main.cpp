#include "Utils/Logger/Logger.hpp"
#include <iostream>

int main() {
    Utils::Logger::Logger &logger = Utils::Logger::Logger::CreateInstance();

    return 0;
}