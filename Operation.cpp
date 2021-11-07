//
// Created by avivs on 11/6/2021.
//

#include <cmath>
#include "Operation.h"


Operation::~Operation() = default;

std::string Operation::to_string() {
    return "<empty operation>";
}

Add::Add(int val) : addVal(val) { }

bool Add::action(int& display) {
    display += addVal;
    return true;
}

std::string Add::to_string() {
    return "+" + std::to_string(addVal);
}

Subtract::Subtract(int val) : subVal(val) { }

bool Subtract::action(int& display) {
    display -= subVal;
    return true;
}

std::string Subtract::to_string() {
    return "-" + std::to_string(subVal);
}

Multiply::Multiply(int val) : mulVal(val) { }

bool Multiply::action(int& display) {
    display *= mulVal;
    return true;
}

std::string Multiply::to_string() {
    return "*" + std::to_string(mulVal);
}

Divide::Divide(int val) : divVal(val) { }

bool Divide::action(int& display) {
    display /= divVal;
    return true;
}

std::string Divide::to_string() {
    return "/" + std::to_string(divVal);
}

Delete::Delete() = default;

bool Delete::action(int& display) {
    display /= 10;
    return true;
}

std::string Delete::to_string() {
    return "<<";
}

Insert::Insert(int val) : insVal(val) { }

bool Insert::action(int& display) {
    display = std::stoi(std::to_string(display) + std::to_string(insVal));
    return true;
}

std::string Insert::to_string() {
    return std::to_string(insVal);
}
