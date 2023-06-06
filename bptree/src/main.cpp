#include <iostream>
#include <string>

#include "BPTree.hpp"

int main() {
    BPTree<std::string, std::string> tree;
    std::string line;
    while (std::getline(std::cin, line)) {
        tree.insert(line, line + line);
    }
    for (const auto& [key, value] : tree) {
        std::cout << key << " => " << value << "\n";
    }
}
