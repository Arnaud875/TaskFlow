#include "Utils/Logger/LoggerFormat.hpp"
#include <iostream>

int main() {
    std::cout << Utils::Logger::FormatStringLogs("Hello my {}, test: {} !", "friend", 24) << std::endl;
    return 0;
}