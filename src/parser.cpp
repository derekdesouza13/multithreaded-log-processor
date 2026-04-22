#include "../include/parser.h"
#include <fstream>
#include <iostream>

std::vector<std::string> read_file(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cout << "Error opening file\n";
        return lines;
    }

    while (getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}