//
// Created by avivs on 11/6/2021.
//

#include <cmath>
#include <stdexcept>
#include "Operation.h"


Operation::~Operation() = default;

std::string Operation::to_string() {
    return "<empty operation>";
}

int Operation::safe_to_int(const std::string& str) {
    static const int int_min_len = std::to_string(INT_MIN).size();

    if (str.size() > int_min_len)
        return INT_MAX;

    long result = std::stol(str);

    if (INT_MIN <= result && result <= INT_MAX)
        return (int) result;
    else
        return INT_MAX;
}

Add::Add(int val) : addVal(val) {}

bool Add::action(int& display) {
    display += addVal;
    return true;
}

std::string Add::to_string() {
    return "+" + std::to_string(addVal);
}

Subtract::Subtract(int val) : subVal(val) {}

bool Subtract::action(int& display) {
    display -= subVal;
    return true;
}

std::string Subtract::to_string() {
    return "-" + std::to_string(subVal);
}

Multiply::Multiply(int val) : mulVal(val) {}

bool Multiply::action(int& display) {
    display *= mulVal;
    return true;
}

std::string Multiply::to_string() {
    return "*" + std::to_string(mulVal);
}

Divide::Divide(int val) : divVal(val) {}

bool Divide::action(int& display) {
    if (display % divVal == 0)
        display /= divVal;
    else
        display = INT_MAX;
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

Insert::Insert(int val) : insVal(val) {}

bool Insert::action(int& display) {
    display = safe_to_int(std::to_string(display) + std::to_string(insVal));
    return true;
}

std::string Insert::to_string() {
    return std::to_string(insVal);
}

Replace::Replace(int val1, int val2) : toReplace(val1), replaceWith(val2) {}

bool Replace::action(int& display) {
    const std::string& target = std::to_string(toReplace);
    const std::string& replacement = std::to_string(replaceWith);
    std::string display_str = std::to_string(display);

    size_t ind = display_str.find(target);
    while (ind != std::string::npos) {
        display_str.replace(ind, target.size(), replacement);
        ind = display_str.find(target, ind + target.size());
    }

    display = safe_to_int(display_str);

    return true;
}

std::string Replace::to_string() {
    return std::to_string(toReplace) + " => " + std::to_string(replaceWith);
}

Reverse::Reverse() {}

bool Reverse::action(int& display) {
    int sign = 1;
    if (display < 0) {
        display *= -1;
        sign = -1;
    }
    std::string s = std::to_string(display);
    std::reverse(s.begin(), s.end());
    display = sign * safe_to_int(s);
    return true;
}

std::string Reverse::to_string() {
    return "Reverse";
}

PlusMinus::PlusMinus() {}

bool PlusMinus::action(int& display) {
    display *= -1;
    return true;
}

std::string PlusMinus::to_string() {
    return "+/-";
}
