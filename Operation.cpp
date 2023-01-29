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

void Add::action(int& display) {
    display += addVal;
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

void Subtract::action(int& display) {
    display -= subVal;
}

void Subtract::mutate_by(const MutatorOperation& op, bool unmutate) {
    op.mutate(subVal, unmutate);
}

std::string Subtract::to_string() {
    return "-" + std::to_string(subVal);
}

Multiply::Multiply(int val) : mulVal(val) {}

void Multiply::action(int& display) {
    display *= mulVal;
}

void Multiply::mutate_by(const MutatorOperation& op, bool unmutate) {
    op.mutate(mulVal, unmutate);
}

std::string Multiply::to_string() {
    return "*" + std::to_string(mulVal);
}

Divide::Divide(int val) : divVal(val) {}

void Divide::action(int& display) {
    if (display % divVal == 0)
        display /= divVal;
    else
        display = INT_MAX;
}

void Divide::mutate_by(const MutatorOperation& op, bool unmutate) {
    op.mutate(divVal, unmutate);
}

std::string Divide::to_string() {
    return "/" + std::to_string(divVal);
}

Delete::Delete() = default;

void Delete::action(int& display) {
    display /= 10;
}

void Delete::mutate_by(const MutatorOperation& op, bool unmutate) {}

std::string Delete::to_string() {
    return "<<";
}

Insert::Insert(int val) : insVal(val) {}

void Insert::action(int& display) {
    display = safe_to_int(std::to_string(display) + std::to_string(insVal));
}

void Insert::mutate_by(const MutatorOperation& op, bool unmutate) {
    op.mutate(insVal, unmutate);
}

std::string Insert::to_string() {
    return std::to_string(insVal);
}

Replace::Replace(int val1, const std::string& val2) : toReplace(val1), replaceWith(val2) {}

void Replace::action(int& display) {
    const std::string& target = std::to_string(toReplace);
    std::string display_str = std::to_string(display);

    size_t ind = display_str.find(target);
    while (ind != std::string::npos) {
        display_str.replace(ind, target.size(), replaceWith);
        ind = display_str.find(target, ind + target.size());
    }

    display = safe_to_int(display_str);
}

void Replace::mutate_by(const MutatorOperation& op, bool unmutate) {
    // removed for now, probably never used
//    op.mutate(toReplace, unmutate);
//    op.mutate(replaceWith, unmutate);
}

std::string Replace::to_string() {
    return std::to_string(toReplace) + " => " + replaceWith;
}

Reverse::Reverse() {}

void Reverse::action(int& display) {
    int sign = 1;
    if (display < 0) {
        display *= -1;
        sign = -1;
    }
    std::string s = std::to_string(display);
    std::reverse(s.begin(), s.end());
    display = sign * safe_to_int(s);
}

void Reverse::mutate_by(const MutatorOperation& op, bool unmutate) {}

std::string Reverse::to_string() {
    return "Reverse";
}

PlusMinus::PlusMinus() {}

void PlusMinus::action(int& display) {
    display *= -1;
}

void PlusMinus::mutate_by(const MutatorOperation& op, bool unmutate) {}

std::string PlusMinus::to_string() {
    return "+/-";
}

Sum::Sum() {}

void Sum::action(int& display) {
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
}

void Sum::mutate_by(const MutatorOperation& op, bool unmutate) {}

std::string Sum::to_string() {
    return "Sum";
}

Cube::Cube() {}

void Cube::action(int& display) {
    int orig_display = display;
    display = orig_display * orig_display * orig_display;
    if ((display / orig_display) / orig_display != orig_display) {
        display = INT_MAX; // overflow occurred
    }
}

void Cube::mutate_by(const MutatorOperation& op, bool unmutate) {}

std::string Cube::to_string() {
    return "x^3";
}

Shift::Shift(bool val) : is_left(val) {}

void Shift::action(int& display) {
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
}

void Shift::mutate_by(const MutatorOperation& op, bool unmutate) {}

std::string Shift::to_string() {
    std::string s = "Shift ";
    return s + (is_left ? "<" : ">");
}

Mirror::Mirror() {}

void Mirror::action(int& display) {
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

void Memory::action(int& display) {
    if (get_memory() == INT_MAX)
        display = INT_MAX;
    else
        // same as insert
        display = safe_to_int(std::to_string(display) + std::to_string(get_memory()));
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

void Store::action(int& display) {
    memory->set_memory(display);
}

void Store::mutate_by(const MutatorOperation& op, bool unmutate) {
    // removed for now, probably never used
//    op.mutate(memory->get_memory(), unmutate);
}

std::string Store::to_string() {
    return "Store";
}

Inv10::Inv10() {}

void Inv10::action(int& display) {
    int sign = 1;
    if (display < 0) {
        display *= -1;
        sign = -1;
    }
    std::string s = std::to_string(display);
    for (char& c : s) {
        if (c != '0')
            c = '0' + (10 - (c - '0'));
    }
    display = sign * safe_to_int(s);
}

void Inv10::mutate_by(const MutatorOperation& op, bool unmutate) {}

std::string Inv10::to_string() {
    return "Inv10";
}
