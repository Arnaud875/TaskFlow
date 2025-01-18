#include <iostream>
#include "Utils/Logger/LoggerFormat.hpp"

int main() {
    std::cout << Utils::Logger::FormatStringLogs("Hello my {} !", "friend") << std::endl;
    return 0;
}