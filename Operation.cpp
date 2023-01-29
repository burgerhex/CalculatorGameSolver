//
// Created by avivs on 11/6/2021.
//

#include <cmath>
#include <stdexcept>
#include <utility>
#include "Operation.h"

Operation::~Operation() = default;

std::string Operation::to_string() {
    return "<empty operation>";
}

MutatorOperation::~MutatorOperation() = default;

std::string MutatorOperation::to_string() {
    return "<empty mutator>";
}

int Operation::safe_to_int(const std::string& str) {
    static const int int_min_len = std::to_string(INT_MIN).size();

    if (str.size() > int_min_len)
        return INT_MAX;

    long long result = std::stoll(str);

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

void Add::mutate_by(const MutatorOperation& op, bool unmutate) {
    op.mutate(addVal, unmutate);
}

std::string Add::to_string() {
    return "+" + std::to_string(addVal);
}

MutatorAdd::MutatorAdd(int val) : addVal(val) {}

void MutatorAdd::mutate(int& val, bool unmutate) const {
    if (unmutate)
        val -= addVal;
    else
        val += addVal;
}

std::string MutatorAdd::to_string() {
    return "[+]" + std::to_string(addVal);
}

Subtract::Subtract(int val) : subVal(val) {}

bool Subtract::action(int& display) {
    display -= subVal;
    return true;
}

void Subtract::mutate_by(const MutatorOperation& op, bool unmutate) {
    op.mutate(subVal, unmutate);
}

std::string Subtract::to_string() {
    return "-" + std::to_string(subVal);
}

Multiply::Multiply(int val) : mulVal(val) {}

bool Multiply::action(int& display) {
    display *= mulVal;
    return true;
}

void Multiply::mutate_by(const MutatorOperation& op, bool unmutate) {
    op.mutate(mulVal, unmutate);
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

void Divide::mutate_by(const MutatorOperation& op, bool unmutate) {
    op.mutate(divVal, unmutate);
}

std::string Divide::to_string() {
    return "/" + std::to_string(divVal);
}

Delete::Delete() = default;

bool Delete::action(int& display) {
    display /= 10;
    return true;
}

void Delete::mutate_by(const MutatorOperation& op, bool unmutate) {}

std::string Delete::to_string() {
    return "<<";
}

Insert::Insert(int val) : insVal(val) {}

bool Insert::action(int& display) {
    display = safe_to_int(std::to_string(display) + std::to_string(insVal));
    return true;
}

void Insert::mutate_by(const MutatorOperation& op, bool unmutate) {
    op.mutate(insVal, unmutate);
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

void Replace::mutate_by(const MutatorOperation& op, bool unmutate) {
    op.mutate(toReplace, unmutate);
    op.mutate(replaceWith, unmutate);
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

void Reverse::mutate_by(const MutatorOperation& op, bool unmutate) {}

std::string Reverse::to_string() {
    return "Reverse";
}

PlusMinus::PlusMinus() {}

bool PlusMinus::action(int& display) {
    display *= -1;
    return true;
}

void PlusMinus::mutate_by(const MutatorOperation& op, bool unmutate) {}

std::string PlusMinus::to_string() {
    return "+/-";
}

Sum::Sum() {}

bool Sum::action(int& display) {
    int sum = 0;
    int sign = 1;
    if (display < 0) {
        display *= -1;
        sign = -1;
    }
    while (display > 0) {
        int d = display / 10; // supposedly more efficient to do this first
        sum += (display % 10);
        display = d;
    }
    display = sign * sum;
    return true;
}

void Sum::mutate_by(const MutatorOperation& op, bool unmutate) {}

std::string Sum::to_string() {
    return "Sum";
}

Cube::Cube() {}

bool Cube::action(int& display) {
    int orig_display = display;
    display = orig_display * orig_display * orig_display;
    if ((display / orig_display) / orig_display != orig_display) {
        display = INT_MAX; // overflow occurred
    }
    return true;
}

void Cube::mutate_by(const MutatorOperation& op, bool unmutate) {}

std::string Cube::to_string() {
    return "x^3";
}

Shift::Shift(bool val) : is_left(val) {}

bool Shift::action(int& display) {
    int sign = 1;
    if (display < 0) {
        display *= -1;
        sign = -1;
    }
    std::string s = std::to_string(display);
    if (is_left) {
        char c = s[0];
        s = s.substr(1) + c;
    } else {
        char c = s.back();
        s = c + s.substr(0, s.size() - 1);
    }
    display = sign * safe_to_int(s);

    return true;
}

void Shift::mutate_by(const MutatorOperation& op, bool unmutate) {}

std::string Shift::to_string() {
    std::string s = "Shift ";
    return s + (is_left ? "<" : ">");
}

Mirror::Mirror() {}

bool Mirror::action(int& display) {
    int sign = 1;
    if (display < 0) {
        display *= -1;
        sign = -1;
    }
    std::string s = std::to_string(display);
    std::string rev = s;
    std::reverse(rev.begin(), rev.end());
    s += rev;
    display = sign * safe_to_int(s);
    return true;
}

void Mirror::mutate_by(const MutatorOperation& op, bool unmutate) {}

std::string Mirror::to_string() {
    return "Mirror";
}

Memory::Memory() : Operation(false, true, false) {}

int Memory::get_memory() {
    if (past_memories.empty()) {
        return INT_MAX;
    } else {
        return past_memories.back();
    }
}

void Memory::set_memory(int memory) {
    past_memories.push_back(memory);
}

void Memory::pop_memory() {
    past_memories.pop_back();
}

bool Memory::action(int& display) {
    if (get_memory() == INT_MAX)
        display = INT_MAX;
    else
        // same as insert
        display = safe_to_int(std::to_string(display) + std::to_string(get_memory()));
    return true;
}

void Memory::mutate_by(const MutatorOperation& op, bool unmutate) {
    // removed for now, probably never used
//    op.mutate(get_memory(), unmutate);
}

std::string Memory::to_string() {
    return std::to_string(get_memory()) + " (recall)";
}

Store::Store(std::shared_ptr<Memory> memory) : Operation(false, false, true),
                                               memory(std::move(memory)) {}

bool Store::action(int& display) {
    memory->set_memory(display);
    return true;
}

void Store::mutate_by(const MutatorOperation& op, bool unmutate) {
    // removed for now, probably never used
//    op.mutate(memory->get_memory(), unmutate);
}

std::string Store::to_string() {
    return "Store";
}
