//
// Created by avivs on 11/6/2021.
//

#include <fstream>
#include <iostream>
#include "Solver.h"

bool is_int(const std::string& s) {
    return std::all_of(s.begin(), s.end(), [](char c) { return isdigit(c); });
}

void Solver::read_file(const std::string& level_file) {
    buttons.clear();

    std::ifstream file(level_file);
    std::string line;

    if (!file.is_open()) {
        std::cout << "Could not find level file" << std::endl;
        return;
    }

    std::getline(file, line);
    moves = std::stoi(line);
    std::getline(file, line);
    start = std::stoi(line);
    std::getline(file, line);
    goal = std::stoi(line);


    while (std::getline(file, line)) {
        Operation* to_add = nullptr;

        if (line.starts_with('+'))
            to_add = new Add(std::stoi(line.substr(1)));
        else if (line.starts_with('-'))
            to_add = new Subtract(std::stoi(line.substr(1)));
        else if (line.starts_with('*'))
            to_add = new Multiply(std::stoi(line.substr(1)));
        else if (line.starts_with('/'))
            to_add = new Divide(std::stoi(line.substr(1)));
        else if (line.starts_with("<<"))
            to_add = new Delete();
        else if (is_int(line))
            to_add = new Insert(std::stoi(line));

        buttons.push_back(to_add);
    }
}