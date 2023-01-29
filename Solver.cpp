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
        std::shared_ptr<Operation> to_add;

        size_t find_replace = line.find("=>");
        size_t find_portal = line.find(',');

        if (line == "+/-" || line == "pm")
            to_add = std::make_shared<PlusMinus>();
        else if (line.starts_with('+'))
            to_add = std::make_shared<Add>(std::stoi(line.substr(1)));
        else if (line.starts_with('-'))
            to_add = std::make_shared<Subtract>(std::stoi(line.substr(1)));
        else if (line.starts_with('*') || line.starts_with('x'))
            to_add = std::make_shared<Multiply>(std::stoi(line.substr(1)));
        else if (line.starts_with('/'))
            to_add = std::make_shared<Divide>(std::stoi(line.substr(1)));
        else if (line.starts_with("[+]"))
            to_add = std::make_shared<MutatorAdd>(std::stoi(line.substr(3)));
        else if (line == "<<" || line == "del")
            to_add = std::make_shared<Delete>();
        else if (find_replace != std::string::npos)
            to_add = std::make_shared<Replace>(std::stoi(line.substr(0, find_replace)),
                                               line.substr(find_replace + 2));
        else if (find_portal != std::string::npos) {
            portal_in = std::stoi(line.substr(0, find_portal)) - 1;
            portal_out = std::stoi(line.substr( find_portal + 1)) - 1;
        } else if (is_int(line))
            to_add = std::make_shared<Insert>(std::stoi(line));
        else if (line == "Reverse" || line == "rev")
            to_add = std::make_shared<Reverse>();
        else if (line == "Sum" || line == "sum")
            to_add = std::make_shared<Sum>();
        else if (line == "Cube" || line == "cube" || line == "x^3")
            to_add = std::make_shared<Cube>();
        else if ((line.starts_with("Shift") || line.starts_with('s')) &&
                 ((line.ends_with('<') || line.ends_with('>'))))
            to_add = std::make_shared<Shift>(line.back() == '<');
        else if (line == "Mirror" || line == "mirror" || line == "mir")
            to_add = std::make_shared<Mirror>();
        else if (line == "Store" || line == "store" || line == "memory" || line == "mem") {
            std::shared_ptr<Memory> memory = std::make_shared<Memory>();
            buttons.push_back(std::make_shared<Store>(memory));
            to_add = memory;
        } else if (line == "Inv10" || line == "inv10" || line == "inv")
            to_add = std::make_shared<Inv10>();
        else
            std::cout << "unrecognizable op " << line << ", ignoring" << std::endl;

        if (to_add)
            buttons.push_back(to_add);
    }

    if (portal_in >= 0 && portal_out >= 0) {
        int p_in = portal_in;
        int p_out = portal_out;
        portal_in_mul = 1;
        portal_out_mul = 1;
        while (p_in--) {
            portal_in_mul *= 10;
        }
        while (p_out--) {
            portal_out_mul *= 10;
        }
    }

    file.close();
}