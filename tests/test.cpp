#include <iostream>
#include <string>

int main() {
    std::string text("Hello, () !");
    std::string concact("World");

    std::size_t pos = text.find("()");
    std::cout << text.substr(0, pos) << std::endl;

    return 0;
}