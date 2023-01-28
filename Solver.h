//
// Created by avivs on 11/6/2021.
//

#ifndef CALCULATORGAMESOLVER_SOLVER_H
#define CALCULATORGAMESOLVER_SOLVER_H


#include <string>
#include <numeric>
#include <vector>
#include <memory>
#include "Operation.h"

class Solver {
private:
    std::vector<std::shared_ptr<Operation>> buttons;
    int start, goal, moves;

public:
    explicit Solver(const std::string& level_file) : start(0), goal(0), moves(0) {
        read_file(level_file);
    }

    int get_start() { return start; }

    int get_goal() { return goal; }

    int get_moves() { return moves; }

    std::vector<std::shared_ptr<Operation>> get_buttons() { return buttons; }

    std::vector<int> get_solution() {
        std::vector<int> buttons_pushed;
        solve_helper(buttons_pushed, start, moves, -1);
        return buttons_pushed;
    }

    static void solve(const std::string& file_name) {
        Solver s(file_name);

        int move = 0;
        int start = s.get_start();
        int goal = s.get_goal();
        int moves = s.get_moves();
        std::vector<std::shared_ptr<Operation>> buttons = s.get_buttons();

        std::cout << "Start: " << start << "\nGoal: " << goal << "\nMoves: " << moves << "\nButtons: ";

        for (int i = 0; i < buttons.size() - 1; i++) {
            std::cout << buttons[i]->to_string() << ", ";
        }

        std::cout << buttons[buttons.size() - 1]->to_string()
                  << "\nAttempting to find solution..." << std::endl;

        std::vector<int> solution = s.get_solution();

        if (solution.empty()) {
            std::cout << "No solution found. :(" << std::endl;
        } else {
            std::cout << "Solution found!" << std::endl;
            int x = start;
            for (int i : solution) {
                std::shared_ptr<Operation> o = buttons[i];
                o->action(x);
                std::cout << "Step " << (++move) << ":\t" << o->to_string() << " --> " << x << std::endl;
            }
        }
    }

private:
    void read_file(const std::string& level_file);

    bool solve_helper(std::vector<int>& buttons_pushed, int display, int moves_left, int button_to_push) {
        std::string s = button_list_to_string(buttons_pushed);
        if (button_to_push >= 0) {
            moves_left -= buttons[button_to_push]->action(display);
#if DEBUG
            std::string indent(moves - moves_left - 1, '\t');
            std::cout << indent << "Trying button " << buttons[button_to_push]->to_string() << " (index "
                      << button_to_push << "), display is now " << display << ". ";
#endif
        }

        if ((moves_left == 0 && display != goal) || display == INT_MAX) {
#if DEBUG
            std::cout << "Out of moves, going up with FALSE.\n";
#endif
            return false;
        } else if (display == goal) {
#if DEBUG
            std::cout << "Goal reached, going up with TRUE.\n";
#endif
            return true;
        } else if (button_to_push >= 0) {
#if DEBUG
            std::cout << "Unsuccessful, continuing.\n";
#endif
        }

        for (int i = 0; i < buttons.size(); i++) {
            buttons_pushed.push_back(i);
            bool result = solve_helper(buttons_pushed, display, moves_left, i);
            if (result) {
                return true;
            }
            buttons_pushed.pop_back();
        }

        return false;
    }

    std::string buttons_list_to_string() {
        std::vector<int> all(buttons.size());
        std::iota(all.begin(), all.end(), 0);
        return button_list_to_string(all);
    }

    std::string button_list_to_string(const std::vector<int>& indices) {
        if (indices.size() == 0)
            return "{}";

        std::string result = "{";

        for (int i = 0; i < indices.size() - 1; i++) {
            result += buttons[indices[i]]->to_string() + ", ";
        }

        return result + buttons[indices[indices.size() - 1]]->to_string() + "}";
    }
};


#endif //CALCULATORGAMESOLVER_SOLVER_H
