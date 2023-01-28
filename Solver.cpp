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
        std::shared_ptr<Operation> to_add = nullptr;

        size_t find_replace = line.find("=>");

        if (line == "+/-")
            to_add = std::make_shared<PlusMinus>();
        else if (line.starts_with('+'))
            to_add = std::make_shared<Add>(std::stoi(line.substr(1)));
        else if (line.starts_with('-'))
            to_add = std::make_shared<Subtract>(std::stoi(line.substr(1)));
        else if (line.starts_with('*'))
            to_add = std::make_shared<Multiply>(std::stoi(line.substr(1)));
        else if (line.starts_with('/'))
            to_add = std::make_shared<Divide>(std::stoi(line.substr(1)));
        else if (line.starts_with("<<"))
            to_add = std::make_shared<Delete>();
        else if (find_replace != std::string::npos)
            to_add = std::make_shared<Replace>(std::stoi(line.substr(0, find_replace)),
                                               std::stoi(line.substr(find_replace + 2)));
        else if (is_int(line))
            to_add = std::make_shared<Insert>(std::stoi(line));
        else if (line == "Reverse" || line == "rev")
            to_add = std::make_shared<Reverse>();

        buttons.push_back(to_add);
    }
}