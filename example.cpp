#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>

#include "getlines.hpp"

int main() {
    std::ifstream input{ "input.txt" };

    for (const auto& line: input | utils::views::getlines) {
        std::cout << line << '\n';
    }
}
